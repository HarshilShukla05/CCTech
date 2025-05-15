#ifndef POLYGON_BOOLEAN_H
#define POLYGON_BOOLEAN_H

#include <QPointF>
#include <vector>

class PolygonBoolean {
public:
    using GHVertex = QPointF;

    // Method to compute the intersection of two polygons
    static std::vector<QPointF> intersect(const std::vector<QPointF>& poly1, const std::vector<QPointF>& poly2);

    // Method to compute the union of two polygons
    static std::vector<QPointF> unionPolygons(const std::vector<QPointF>& poly1, const std::vector<QPointF>& poly2);

    // Method to compute the subtraction of two polygons
    static std::vector<QPointF> subtractPolygons(const std::vector<QPointF>& poly1, const std::vector<QPointF>& poly2);

   
};
 bool pointInPolygon(const QPointF& point, const std::vector<QPointF>& polygon);
#endif // POLYGON_BOOLEAN_H
