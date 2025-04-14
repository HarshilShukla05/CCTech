#include "polygon.h"
#include "plot_utils.h"
#include <fstream>
#include <iostream>
#include <cmath>

using namespace std;

Polygon::Polygon(int sides, double radius) {
    vertices.reserve(sides);
    generateRegularPolygon(sides, radius);
}

// Generate vertices for a regular polygon based on the number of sides and radius
void Polygon::generateRegularPolygon(int sides, double radius) {
    double angleIncrement = 2 * M_PI / sides;  // Angle between adjacent vertices
    for (int i = 0; i < sides; ++i) {
        double angle = i * angleIncrement;
        double x = radius * cos(angle);  // X-coordinate of the vertex
        double y = radius * sin(angle);  // Y-coordinate of the vertex
        double z = 0.0;  // Assuming a 2D polygon (on the XY plane)
        addVertex(x, y, z);
    }
}

void Polygon::addVertex(double x, double y, double z) {
    vertices.push_back({x, y, z});
}

// centroid
void Polygon::calculateCentroid(double &cx, double &cy, double &cz) {
    cx = cy = cz = 0;
    for (const auto &v : vertices) {
        cx += v[0];
        cy += v[1];
        cz += v[2];
    }
    cx /= vertices.size();
    cy /= vertices.size();
    cz /= vertices.size();
}

void Polygon::plot(const string &filename) const {
    saveToFile(filename);
    PlotUtils::plot(filename, "Polygon", "blue");
}

void Polygon::translate(double dx, double dy, double dz) {
    PlotUtils::translate(vertices, dx, dy, dz);
}

void Polygon::scale(double sx, double sy, double sz) {
    PlotUtils::scale(vertices, sx, sy, sz);
}

void Polygon::scale(double factor) {
    scale(factor, factor, factor); 
}

void Polygon::rotate(double angle, char axis) {
    double cx, cy, cz;
    calculateCentroid(cx, cy, cz);
    PlotUtils::rotate(vertices, angle, axis, cx, cy, cz);
}

void Polygon::saveToFile(const string &filename) const {
    ofstream file(filename, ios::app);
    if (!file) {
        cerr << "Error: Cannot open file for writing.\n";
        return;
    }
    file << "Polygon " << vertices.size() << "\n";
    for (const auto &v : vertices) {
        file << v[0] << " " << v[1] << " " << v[2] << "\n";
    }
    file.close();
}

void Polygon::loadFromFile(const string &filename) {
    ifstream file(filename);
    if (!file) {
        cerr << "Error: Cannot open file for reading.\n";
        return;
    }
    vertices.clear();
    int numVertices;
    file >> numVertices;
    for (int i = 0; i < numVertices; ++i) {
        double x, y, z;
        file >> x >> y >> z;
        vertices.push_back({x, y, z});
    }
    file.close();
}

std::vector<std::vector<double>> Polygon::getEdgeLines() const {
    std::vector<std::vector<double>> edgeLines;

    if (vertices.size() < 2) return edgeLines;

    for (size_t i = 0; i < vertices.size(); ++i) {
        const auto& start = vertices[i];
        const auto& end = vertices[(i + 1) % vertices.size()]; // loop back to close the polygon

        edgeLines.push_back(start);
        edgeLines.push_back(end);
    }

    return edgeLines;
}
