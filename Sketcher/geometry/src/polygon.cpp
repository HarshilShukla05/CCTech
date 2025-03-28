#include "polygon.h"
#include <fstream>
#include <iostream>
#include <cmath>

using namespace std;

// initialize polygon with a given number of sides
Polygon::Polygon(int sides) {
    vertices.reserve(sides);
}

// Add a vertex to the polygon
void Polygon::addVertex(double x, double y, double z) {
    vertices.push_back({x, y, z});
}

// centroid of the polygon (for rotation)
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

// Plot the polygon using GNUPlot
void Polygon::plot(const string &filename) const {
    ofstream file(filename);
    if (!file) {
        cerr << "Error opening file: " << filename << "\n";
        return;
    }

    for (const auto &v : vertices) {
        file << v[0] << " " << v[1] << " " << v[2] << "\n";
    }
    file << vertices[0][0] << " " << vertices[0][1] << " " << vertices[0][2] << "\n"; // Close the polygon
    file.close();

    string command = 
        "gnuplot -p -e \""
        "set terminal wxt; "
        "set xlabel 'X'; "
        "set ylabel 'Y'; "
        "set zlabel 'Z'; "
        "plot '" + filename + "' with linespoints pointtype 7 linecolor 'blue'\"";

    int result = system(command.c_str());
    if (result != 0) {
        cerr << "Error: Failed to execute GNUPLOT command.\n";
    }
}

// Translate the polygon
void Polygon::translate(double dx, double dy, double dz) {
    for (auto &v : vertices) {
        v[0] += dx;
        v[1] += dy;
        v[2] += dz;
    }
}

// Scale the polygon
void Polygon::scale(double sx, double sy, double sz) {
    for (auto &v : vertices) {
        v[0] *= sx;
        v[1] *= sy;
        v[2] *= sz;
    }
}

void Polygon::scale(double factor) {
    scale(factor, factor, factor); // Delegate to multi-factor scaling
}

// Rotate the polygon around its centroid
void Polygon::rotate(double angle, char axis) {
    double radians = angle * M_PI / 180.0;
    double cx, cy, cz;
    calculateCentroid(cx, cy, cz);

    for (auto &v : vertices) {
        double x = v[0] - cx, y = v[1] - cy, z = v[2] - cz;
        if (axis == 'x') {
            v[1] = cy + y * cos(radians) - z * sin(radians);
            v[2] = cz + y * sin(radians) + z * cos(radians);
        } else if (axis == 'y') {
            v[0] = cx + x * cos(radians) + z * sin(radians);
            v[2] = cz - x * sin(radians) + z * cos(radians);
        } else if (axis == 'z') {
            v[0] = cx + x * cos(radians) - y * sin(radians);
            v[1] = cy + x * sin(radians) + y * cos(radians);
        }
    }
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
