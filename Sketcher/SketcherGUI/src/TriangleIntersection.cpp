#include "TriangleIntersection.h"
#include <cmath>

bool rayIntersectsTriangle(const QVector3D& orig, const QVector3D& dir,
                           const Triangle& tri, QVector3D& outIntersection) {
    const float EPSILON = 1e-6;
    QVector3D edge1 = tri.v1 - tri.v0;
    QVector3D edge2 = tri.v2 - tri.v0;
    QVector3D h = QVector3D::crossProduct(dir, edge2);
    float a = QVector3D::dotProduct(edge1, h);

    if (std::abs(a) < EPSILON)
        return false;

    float f = 1.0f / a;
    QVector3D s = orig - tri.v0;
    float u = f * QVector3D::dotProduct(s, h);

    if (u < 0.0 || u > 1.0)
        return false;

    QVector3D q = QVector3D::crossProduct(s, edge1);
    float v = f * QVector3D::dotProduct(dir, q);

    if (v < 0.0 || u + v > 1.0)
        return false;

    float t = f * QVector3D::dotProduct(edge2, q);
    if (t > EPSILON) {
        outIntersection = orig + dir * t;
        return true;
    }
    return false;
}

bool triangleIntersectsTriangle(const Triangle& t1, const Triangle& t2,
                                std::vector<QVector3D>& intersections) {
    QVector3D temp;
    QVector3D edges1[] = { t1.v1 - t1.v0, t1.v2 - t1.v1, t1.v0 - t1.v2 };
    QVector3D starts1[] = { t1.v0, t1.v1, t1.v2 };

    for (int i = 0; i < 3; ++i) {
        if (rayIntersectsTriangle(starts1[i], edges1[i], t2, temp)) {
            intersections.push_back(temp);
        }
    }

    QVector3D edges2[] = { t2.v1 - t2.v0, t2.v2 - t2.v1, t2.v0 - t2.v2 };
    QVector3D starts2[] = { t2.v0, t2.v1, t2.v2 };

    for (int i = 0; i < 3; ++i) {
        if (rayIntersectsTriangle(starts2[i], edges2[i], t1, temp)) {
            intersections.push_back(temp);
        }
    }

    return !intersections.empty();
}
