#include "polygon_boolean.h"
#include <QDebug>
#include <algorithm>
#include "clipper.hpp"

using namespace ClipperLib;

using namespace std;

using Vertex = PolygonBoolean::GHVertex;

// Helper function to check if a point is inside a polygon
bool pointInPolygon(const QPointF& point, const vector<QPointF>& polygon) {
    int count = 0;
    for (size_t i = 0; i < polygon.size(); ++i) {
        QPointF a = polygon[i];
        QPointF b = polygon[(i + 1) % polygon.size()];
        if ((a.y() > point.y()) != (b.y() > point.y())) {
            double intersectX = (b.x() - a.x()) * (point.y() - a.y()) / (b.y() - a.y()) + a.x();
            if (point.x() < intersectX) {
                count++;
            }
        }
    }
    return (count % 2) == 1;
}

// Helper function to find intersection of two line segments
bool segmentsIntersect(const QPointF& a1, const QPointF& a2, const QPointF& b1, const QPointF& b2, QPointF& out) {
    double dx1 = a2.x() - a1.x();
    double dy1 = a2.y() - a1.y();
    double dx2 = b2.x() - b1.x();
    double dy2 = b2.y() - b1.y();

    double denom = dx1 * dy2 - dy1 * dx2;
    if (abs(denom) < 1e-8) return false; // parallel

    double ua = ((b1.x() - a1.x()) * dy2 - (b1.y() - a1.y()) * dx2) / denom;
    double ub = ((b1.x() - a1.x()) * dy1 - (b1.y() - a1.y()) * dx1) / denom;

    if (ua < 0 || ua > 1 || ub < 0 || ub > 1) return false;

    out = QPointF(a1.x() + ua * dx1, a1.y() + ua * dy1);
    return true;
}

// Helper function to add intersection points and sort them
void addAndSortIntersections(vector<QPointF>& result, const QPointF& point) {
    if (find(result.begin(), result.end(), point) == result.end()) {
        result.push_back(point);
    }
}

// Helper function to split edges at intersection points
vector<QPointF> splitEdges(const vector<QPointF>& polygon, const vector<QPointF>& intersections) {
    vector<QPointF> result;
    for (size_t i = 0; i < polygon.size(); ++i) {
        QPointF a = polygon[i];
        QPointF b = polygon[(i + 1) % polygon.size()];
        result.push_back(a);

        for (const auto& inter : intersections) {
            QPointF dummy;
            if (segmentsIntersect(a, b, inter, inter, dummy)) {
                result.push_back(inter);
            }
        }
    }

    // Sort points along each edge
    sort(result.begin(), result.end(), [](const QPointF& p1, const QPointF& p2) {
        return p1.x() < p2.x() || (p1.x() == p2.x() && p1.y() < p2.y());
    });

    return result;
}

namespace {
    // Helper function to compare two QPointF objects with a tolerance
    bool pointFuzzyEqual(const QPointF& p1, const QPointF& p2, double tolerance = 1e-6) {
        return std::abs(p1.x() - p2.x()) < tolerance && std::abs(p1.y() - p2.y()) < tolerance;
    }
}

// Refined intersection implementation
vector<QPointF> PolygonBoolean::intersect(const vector<QPointF>& poly1, const vector<QPointF>& poly2) {
    vector<QPointF> intersections;

    // Find all intersection points
    for (size_t i = 0; i < poly1.size(); ++i) {
        QPointF a1 = poly1[i];
        QPointF a2 = poly1[(i + 1) % poly1.size()];
        for (size_t j = 0; j < poly2.size(); ++j) {
            QPointF b1 = poly2[j];
            QPointF b2 = poly2[(j + 1) % poly2.size()];
            QPointF inter;
            if (segmentsIntersect(a1, a2, b1, b2, inter)) {
                addAndSortIntersections(intersections, inter);
            }
        }
    }

    // Add points from poly1 inside poly2
    for (const auto& point : poly1) {
        if (pointInPolygon(point, poly2)) {
            addAndSortIntersections(intersections, point);
        }
    }

    // Add points from poly2 inside poly1
    for (const auto& point : poly2) {
        if (pointInPolygon(point, poly1)) {
            addAndSortIntersections(intersections, point);
        }
    }

    // Sort points in clockwise order
    QPointF centroid(0, 0);
    for (const auto& point : intersections) {
        centroid += point;
    }
    centroid /= intersections.size();

    sort(intersections.begin(), intersections.end(), [&centroid](const QPointF& a, const QPointF& b) {
        double angleA = atan2(a.y() - centroid.y(), a.x() - centroid.x());
        double angleB = atan2(b.y() - centroid.y(), b.x() - centroid.x());
        return angleA < angleB;
    });

    return intersections;
}

// Refined union implementation
vector<QPointF> PolygonBoolean::unionPolygons(const vector<QPointF>& poly1, const vector<QPointF>& poly2) {
    vector<QPointF> intersections;

    // Find all intersection points
    for (size_t i = 0; i < poly1.size(); ++i) {
        QPointF a1 = poly1[i];
        QPointF a2 = poly1[(i + 1) % poly1.size()];
        for (size_t j = 0; j < poly2.size(); ++j) {
            QPointF b1 = poly2[j];
            QPointF b2 = poly2[(j + 1) % poly2.size()];
            QPointF inter;
            if (segmentsIntersect(a1, a2, b1, b2, inter)) {
                addAndSortIntersections(intersections, inter);
            }
        }
    }

    // Split edges at intersection points
    vector<QPointF> splitPoly1 = splitEdges(poly1, intersections);
    vector<QPointF> splitPoly2 = splitEdges(poly2, intersections);

    // Construct union polygon
    vector<QPointF> result;

    // Add points from poly1 that are outside poly2
    for (const auto& point : splitPoly1) {
        if (!pointInPolygon(point, poly2)) {
            result.push_back(point);
        }
    }

    // Add points from poly2 that are outside poly1
    for (const auto& point : splitPoly2) {
        if (!pointInPolygon(point, poly1)) {
            result.push_back(point);
        }
    }

    // Add intersection points
    for (const auto& inter : intersections) {
        result.push_back(inter);
    }

    // Sort points in clockwise order
    QPointF centroid(0, 0);
    for (const auto& point : result) {
        centroid += point;
    }
    centroid /= result.size();

    sort(result.begin(), result.end(), [&centroid](const QPointF& a, const QPointF& b) {
        double angleA = atan2(a.y() - centroid.y(), a.x() - centroid.x());
        double angleB = atan2(b.y() - centroid.y(), b.x() - centroid.x());
        return angleA < angleB;
    });

    // Remove duplicate points
    result.erase(unique(result.begin(), result.end(), [](const QPointF& a, const QPointF& b) {
        return qFuzzyCompare(a.x(), b.x()) && qFuzzyCompare(a.y(), b.y());
    }), result.end());

    return result;
}

//  subtraction implementation through Clipper library
std::vector<QPointF> PolygonBoolean::subtractPolygons(const std::vector<QPointF>& polyA, const std::vector<QPointF>& polyB) {
    Path subject, clip;
    for (const QPointF& pt : polyA)
        subject << IntPoint(static_cast<cInt>(pt.x() * 1000), static_cast<cInt>(pt.y() * 1000));
    for (const QPointF& pt : polyB)
        clip << IntPoint(static_cast<cInt>(pt.x() * 1000), static_cast<cInt>(pt.y() * 1000));

    Clipper c;
    c.AddPath(subject, ptSubject, true);
    c.AddPath(clip, ptClip, true);
    Paths solution;
    c.Execute(ctDifference, solution, pftNonZero, pftNonZero);

    std::vector<QPointF> result;
    if (!solution.empty()) {
        for (const IntPoint& ip : solution.front())
            result.emplace_back(ip.X / 1000.0, ip.Y / 1000.0);
    }
    return result;
}



