#ifndef EXTRUDEUTILS_H
#define EXTRUDEUTILS_H

#include <QPointF>
#include <QVector3D>
#include <vector>
#include <array>
#include <QPolygonF>

namespace ExtrudeUtils {
std::pair<std::vector<std::vector<double>>, std::vector<std::array<int, 3>>>
extrudePolygon(const std::vector<QPointF>& poly, double height);

void generateExtrusion(const QPolygonF& region, float depth, std::vector<QVector3D>& vertices, std::vector<unsigned int>& indices);
}

#endif // EXTRUDEUTILS_H
