#ifndef TRIANGLE_INTERSECTION_H
#define TRIANGLE_INTERSECTION_H

#include <QVector3D>
#include <vector>

struct Triangle {
    QVector3D v0, v1, v2;
};

bool rayIntersectsTriangle(const QVector3D& orig, const QVector3D& dir,
                           const Triangle& tri, QVector3D& outIntersection);

bool triangleIntersectsTriangle(const Triangle& t1, const Triangle& t2,
                                std::vector<QVector3D>& intersections);

#endif
