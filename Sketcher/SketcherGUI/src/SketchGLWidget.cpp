#include "SketchGLWidget.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <QOpenGLFunctions> // <-- This is correct; do not use <OpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QVector3D>
#include <vector>
#include "polygon_boolean.h"
#include "ExtrudeUtils.h" // Include extrusion utilities

#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>

SketchGLWidget::SketchGLWidget(QWidget* parent)
    : QOpenGLWidget(parent), selectedRegionIndex(-1), resultColor(Qt::transparent), bezierMode(false), hoveredIntersectionIndex(-1) 
    {
        setMouseTracking(true);
        setFocusPolicy(Qt::StrongFocus);
        resultRegions.clear();
    }

void SketchGLWidget::initializeGL() {
    QOpenGLContext::currentContext()->functions()->initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);
    glClearColor(1, 1, 1, 1);  // White background
}

void SketchGLWidget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
}

void SketchGLWidget::paintGL() {
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QPainter painter(this);
     painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    // Draw existing curves
    painter.setPen(QPen(Qt::black, 2));
    for (const auto& curveList : shapeCurves) {
        for (const auto& segment : curveList) {
            if (segment.type == CurveSegment::Line) {
                painter.drawLine(segment.p0, segment.p3);
            } else if (segment.type == CurveSegment::CubicBezier) {
                drawBezierCurve(segment, painter);
            }
        }
    }

    // Draw live Bézier preview while placing control points
    if (drawMode == DrawMode::Bezier && !bezierControlPoints.empty()) {
        painter.setPen(QPen(Qt::blue, 1, Qt::DashLine));
        // Draw lines between control points
        for (size_t i = 0; i + 1 < bezierControlPoints.size(); ++i) {
            painter.drawLine(bezierControlPoints[i], bezierControlPoints[i + 1]);
        }

        // Draw the current Bézier preview curve with partial points
        if (bezierControlPoints.size() >= 2) {
            CurveSegment tempBezier;
            tempBezier.type = CurveSegment::CubicBezier;
            tempBezier.p0 = bezierControlPoints[0];
            tempBezier.p1 = bezierControlPoints.size() > 1 ? bezierControlPoints[1] : bezierControlPoints[0];
            tempBezier.p2 = bezierControlPoints.size() > 2 ? bezierControlPoints[2] : tempBezier.p1;
            tempBezier.p3 = bezierControlPoints.size() > 3 ? bezierControlPoints[3] : tempBezier.p2;

            auto previewPoints = flattenBezier(tempBezier, 30);
            for (size_t i = 0; i + 1 < previewPoints.size(); ++i) {
                painter.drawLine(previewPoints[i], previewPoints[i + 1]);
            }
        }
    }

    // Draw intersection points with hover effect and labels
    painter.setPen(Qt::NoPen);
    for (size_t i = 0; i < bezierIntersections.size(); ++i) {
        const QPointF& pt = bezierIntersections[i];
        bool isHovered = (static_cast<int>(i) == hoveredIntersectionIndex);
        int radius = isHovered ? 10 : 6;

        // Red circle
        painter.setPen(Qt::NoPen);
        painter.setBrush(Qt::red);
        painter.drawEllipse(pt, radius, radius);

        // White crosshair
        painter.setPen(QPen(Qt::white, isHovered ? 3 : 2));
        painter.drawLine(pt.x() - radius, pt.y(), pt.x() + radius, pt.y());
        painter.drawLine(pt.x(), pt.y() - radius, pt.x(), pt.y() + radius);

        // Label
        QFont font = painter.font();
        font.setPointSize(isHovered ? 10 : 8);
        font.setBold(isHovered);
        painter.setFont(font);
        painter.setPen(Qt::black);

        QPointF offset(radius + 6, -radius - 4);
        painter.drawText(pt + offset, QString::number(i + 1));
    }

    // Draw existing polygon shapes
    QPen pen(Qt::black, 2);
    painter.setPen(pen);
    for (const auto& shape : shapes) {
        for (size_t i = 0; i < shape.size(); ++i) {
            painter.drawLine(shape[i], shape[(i + 1) % shape.size()]);
        }
    }

    // Draw current polygon points
    painter.setPen(QPen(Qt::red, 2));
    for (size_t i = 0; i + 1 < currentPoints.size(); ++i) {
        painter.drawLine(currentPoints[i], currentPoints[i + 1]);
    }

    // Draw result polygon regions
    for (int i = 0; i < resultRegions.size(); ++i) {
        QBrush brush(i == selectedRegionIndex ? QColor(255, 255, 0, 120) : QColor(100, 100, 255, 100));
        painter.setBrush(brush);
        QPolygonF poly;
        for (const auto& pt : resultRegions[i]) {
            poly << pt;
        }
        painter.drawPolygon(poly);
    }
    painter.end();

    // Draw extruded geometry if available
    if (!extrudedVertices.empty() && !extrudedIndices.empty()) {
        glEnable(GL_DEPTH_TEST);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(45.0, double(width()) / height(), 1.0, 1000.0);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(0, 0, -300.0f); // Camera distance
        glRotatef(30, 1, 0, 0);      // Tilt
        glRotatef(45, 0, 1, 0);      // Rotate

        glColor3f(0.2f, 0.7f, 0.9f); // Cyan
        glBegin(GL_TRIANGLES);
        for (size_t i = 0; i < extrudedIndices.size(); i += 3) {
            for (int j = 0; j < 3; ++j) {
                const auto& v = extrudedVertices[extrudedIndices[i + j]];
                glVertex3f(v.x(), v.y(), v.z());
            }
        }
        glEnd();

        glDisable(GL_DEPTH_TEST);
    }
}



std::vector<QVector3D> extrudedVertices; // Declare extruded vertices
std::vector<unsigned int> extrudedIndices; // Declare extruded indices

void SketchGLWidget::drawExtrudedGeometry() {
    if (extrudedVertices.empty() || extrudedIndices.empty()) return;

    QOpenGLFunctions* f = QOpenGLContext::currentContext()->functions();
    f->glEnableVertexAttribArray(0);
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, extrudedVertices.data());
    f->glDrawElements(GL_TRIANGLES, extrudedIndices.size(), GL_UNSIGNED_INT, extrudedIndices.data());
    f->glDisableVertexAttribArray(0);
}

bool pointFuzzyEqual(const QPointF& a, const QPointF& b, double eps) {
    return std::abs(a.x() - b.x()) < eps && std::abs(a.y() - b.y()) < eps;
}


void SketchGLWidget::mousePressEvent(QMouseEvent* event) {
    QPointF clicked = event->pos();
    selectedRegionIndex = -1;
    for (int i = 0; i < resultRegions.size(); ++i) {
        if (pointInPolygon(clicked, resultRegions[i])) {
            selectedRegionIndex = i;
            update();
            return;
        }
    }

    if (drawMode == DrawMode::Bezier) {
        bezierControlPoints.push_back(clicked);
        if (bezierControlPoints.size() == 4) {
            CurveSegment bezier;
            bezier.type = CurveSegment::CubicBezier;
            bezier.p0 = bezierControlPoints[0];
            bezier.p1 = bezierControlPoints[1];
            bezier.p2 = bezierControlPoints[2];
            bezier.p3 = bezierControlPoints[3];

            shapeCurves.push_back({ bezier });
            bezierControlPoints.clear();
            update();
        } else {
            // Live preview update after each control point added
            update();
        }
    } else {
        if (!currentPoints.empty() && pointFuzzyEqual(clicked, currentPoints.front())) {
            finishCurrentShape();
        } else {
            currentPoints.push_back(clicked);
        }
        update();
    }
}

void SketchGLWidget::mouseMoveEvent(QMouseEvent* event) {
    QPointF mousePos = event->pos();
    hoveredIntersectionIndex = -1;
    const double hoverRadius = 10.0; // pixels

    for (size_t i = 0; i < bezierIntersections.size(); ++i) {
        if (QLineF(mousePos, bezierIntersections[i]).length() <= hoverRadius) {
            hoveredIntersectionIndex = static_cast<int>(i);
            break;
        }
    }
    update(); // repaint with hover effect
}


void SketchGLWidget::drawBezierCurve(const CurveSegment& bezier, QPainter& painter) {
    std::vector<QPointF> points = flattenBezier(bezier, 30);
    for (size_t i = 0; i + 1 < points.size(); ++i)
        painter.drawLine(points[i], points[i + 1]);
}

std::vector<QPointF> SketchGLWidget::flattenBezier(const CurveSegment& bez, int segments) {
    std::vector<QPointF> points;
    for (int i = 0; i <= segments; ++i) {
        double t = double(i) / segments;
        double u = 1 - t;

        QPointF pt = 
            bez.p0 * (u * u * u) +
            bez.p1 * (3 * u * u * t) +
            bez.p2 * (3 * u * t * t) +
            bez.p3 * (t * t * t);

        points.push_back(pt);
    }
    return points;
}


void SketchGLWidget::intersectBezierRecursive(const CurveSegment& a, const CurveSegment& b, std::vector<QPointF>& intersections, int depth) {
    if (depth > 10) return;

    QRectF boxA = QRectF(a.p0, a.p3).normalized().adjusted(-1, -1, 1, 1).united(QRectF(a.p1, a.p2));
    QRectF boxB = QRectF(b.p0, b.p3).normalized().adjusted(-1, -1, 1, 1).united(QRectF(b.p1, b.p2));
    if (!boxA.intersects(boxB)) return;

    if (QLineF(a.p0, a.p3).length() < 2 && QLineF(b.p0, b.p3).length() < 2) {
        QPointF mid = (a.p0 + b.p3) / 2.0;
        intersections.push_back(mid);
        return;
    }

    // Subdivide
    auto subdivide = [](const CurveSegment& seg) -> std::pair<CurveSegment, CurveSegment> {
        QPointF m01 = (seg.p0 + seg.p1) / 2;
        QPointF m12 = (seg.p1 + seg.p2) / 2;
        QPointF m23 = (seg.p2 + seg.p3) / 2;
        QPointF m012 = (m01 + m12) / 2;
        QPointF m123 = (m12 + m23) / 2;
        QPointF m0123 = (m012 + m123) / 2;

        CurveSegment left = { CurveSegment::CubicBezier, seg.p0, m01, m012, m0123 };
        CurveSegment right = { CurveSegment::CubicBezier, m0123, m123, m23, seg.p3 };
        return { left, right };
    };

    auto [a1, a2] = subdivide(a);
    auto [b1, b2] = subdivide(b);

    intersectBezierRecursive(a1, b1, intersections, depth + 1);
    intersectBezierRecursive(a1, b2, intersections, depth + 1);
    intersectBezierRecursive(a2, b1, intersections, depth + 1);
    intersectBezierRecursive(a2, b2, intersections, depth + 1);
}

void SketchGLWidget::toggleBezierMode() {
    drawMode = (drawMode == DrawMode::Line) ? DrawMode::Bezier : DrawMode::Line;
    bezierMode = (drawMode == DrawMode::Bezier);
    bezierControlPoints.clear();
    update();
}

void SketchGLWidget::findBezierIntersections() {
    bezierIntersections.clear();
    for (size_t i = 0; i < shapeCurves.size(); ++i) {
        for (const auto& a : shapeCurves[i]) {
            if (a.type != CurveSegment::CubicBezier) continue;
            for (size_t j = i + 1; j < shapeCurves.size(); ++j) {
                for (const auto& b : shapeCurves[j]) {
                    if (b.type != CurveSegment::CubicBezier) continue;
                    intersectBezierRecursive(a, b, bezierIntersections);
                }
            }
        }
    }
    update();
}

void SketchGLWidget::finishCurrentShape() {
    if (currentPoints.size() < 3) return;

    shapes.push_back(currentPoints);
    qDebug() << "Added shape with" << currentPoints.size()-1 << "points.";
    currentPoints.clear();
    update();
}

void SketchGLWidget::applyUnion() {
    if (shapes.size() < 2) return;
    const auto& poly1 = shapes[0];
    const auto& poly2 = shapes[1];
    auto result = PolygonBoolean::unionPolygons(poly1, poly2);
    if (!result.empty()) {
        resultRegions.clear();
        resultRegions.push_back(result);
        selectedRegionIndex = 0;
        update();
    }
}

void SketchGLWidget::applyIntersection() {
    if (shapes.size() < 2) return;
    const auto& poly1 = shapes[0];
    const auto& poly2 = shapes[1];
    auto result = PolygonBoolean::intersect(poly1, poly2);
    if (!result.empty()) {
        resultRegions.clear();
        resultRegions.push_back(result);
        selectedRegionIndex = 0;
        update();
    }
}

void SketchGLWidget::applySubtraction(bool isAB) {
    if (shapes.size() < 2) return;
    const auto& poly1 = isAB ? shapes[0] : shapes[1];
    const auto& poly2 = isAB ? shapes[1] : shapes[0];
    auto result = PolygonBoolean::subtractPolygons(poly1, poly2);
    if (!result.empty()) {
        shapes = { result };
        resultRegions.clear();
        selectedRegionIndex = -1;
        update();
    }
}

void SketchGLWidget::clearResult() {
    resultRegions.clear();
    selectedRegionIndex = -1;
    resultColor = Qt::transparent;
    update();
}

std::vector<std::vector<QPointF>> SketchGLWidget::getPolygons() const {
    return shapes;
}

std::vector<QPointF> SketchGLWidget::getSelectedPolygon() const {
    if (selectedRegionIndex >= 0 && selectedRegionIndex < resultRegions.size())
        return resultRegions[selectedRegionIndex];
    return {};
}

void SketchGLWidget::setExtrudedData(const std::vector<QVector3D>& verts, const std::vector<unsigned int>& indices) {
    extrudedVertices = verts;
    extrudedIndices = indices;
}

void SketchGLWidget::extrudeSelectedRegion(float depth) {
    if (resultRegions.empty()) {
        qDebug() << "No regions available for extrusion.";
        return;
    }

    std::vector<QVector3D> allVertices;
    std::vector<unsigned int> allIndices;
    unsigned int vertexOffset = 0;

    for (const auto& region : resultRegions) {
        if (region.size() < 3) continue;

        QPolygonF polygon;
        for (const auto& pt : region)
            polygon << pt;

        std::vector<QVector3D> regionVerts;
        std::vector<unsigned int> regionIndices;

        ExtrudeUtils::generateExtrusion(polygon, depth, regionVerts, regionIndices);

        // Offset indices to match global vertex list
        for (auto& idx : regionIndices)
            idx += vertexOffset;

        allVertices.insert(allVertices.end(), regionVerts.begin(), regionVerts.end());
        allIndices.insert(allIndices.end(), regionIndices.begin(), regionIndices.end());

        vertexOffset += regionVerts.size();
    }

    setExtrudedData(allVertices, allIndices); 

    qDebug() << "Extruded" << resultRegions.size() << "regions into"
             << allVertices.size() << "vertices and"
             << allIndices.size() << "indices.";

    update(); // triggers paintGL()
}

bool SketchGLWidget::isBezierModeActive() const {
    return bezierMode;
}


