#include "polygon.h"
#include "plot_utils.h"
#include <fstream>
#include <iostream>
#include <cmath>

using namespace std;

Polygon::Polygon(int sides) {
    vertices.reserve(sides);
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
