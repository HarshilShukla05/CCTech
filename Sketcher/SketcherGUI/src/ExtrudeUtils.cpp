#include "ExtrudeUtils.h"

#include <vector>
#include <array>
#include <utility>

std::pair<std::vector<std::vector<double>>, std::vector<std::array<int, 3>>>
ExtrudeUtils::extrudePolygon(const std::vector<QPointF>& poly, double height) {
    std::vector<std::vector<double>> vertices;
    std::vector<std::array<int, 3>> faces;

    int n = poly.size();
    for (const auto& pt : poly)
        vertices.push_back({pt.x(), pt.y(), 0.0});
    for (const auto& pt : poly)
        vertices.push_back({pt.x(), pt.y(), height});

    for (int i = 0; i < n; ++i) {
        int a = i;
        int b = (i + 1) % n;
        int aTop = a + n;
        int bTop = b + n;

        faces.push_back({a, b, aTop});
        faces.push_back({b, bTop, aTop});
    }

    return {vertices, faces};
}

void ExtrudeUtils::generateExtrusion(const QPolygonF &region, float depth, std::vector<QVector3D> &vertices, std::vector<unsigned int> &indices) {
    vertices.clear();
    indices.clear();

    // Create top and bottom faces
    for (const QPointF &point : region) {
        vertices.emplace_back(point.x(), point.y(), 0.0f); // Bottom face
        vertices.emplace_back(point.x(), point.y(), depth); // Top face
    }

    int vertexCount = region.size();

    // Create side faces
    for (int i = 0; i < vertexCount; ++i) {
        int next = (i + 1) % vertexCount;

        // Bottom face triangle
        indices.push_back(i * 2);
        indices.push_back(next * 2);
        indices.push_back(i * 2 + 1);

        // Top face triangle
        indices.push_back(i * 2 + 1);
        indices.push_back(next * 2);
        indices.push_back(next * 2 + 1);
    }
}