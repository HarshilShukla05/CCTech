#include "sphere.h"
#include "plot_utils.h"
#include <fstream>
#include <cmath>
#include <iostream>

using namespace std;

Sphere::Sphere(double r, int seg) : radius(r), segments(seg) {
    for (int i = 0; i <= segments; i++) {
        double theta = M_PI * i / segments;
        for (int j = 0; j < 2 * segments; j++) {
            double phi = 2 * M_PI * j / (2 * segments);
            double x = radius * sin(theta) * cos(phi);
            double y = radius * sin(theta) * sin(phi);
            double z = radius * cos(theta);
            vertices.push_back({x, y, z});
        }
    }
}

void Sphere::translate(double dx, double dy, double dz) {
    PlotUtils::translate(vertices, dx, dy, dz);
}

void Sphere::scale(double sx, double sy, double sz) {
    PlotUtils::scale(vertices, sx, sy, sz);
}

void Sphere::scale(double factor) {
    scale(factor, factor, factor);
}

void Sphere::rotate(double angle, char axis) {
    // Rotation does not affect the geometry of a sphere
}

void Sphere::plot(const string& filename) const {
    saveToFile(filename);
    PlotUtils::plot(filename, "Sphere", "blue");
}

void Sphere::saveToFile(const string &filename) const {
    ofstream file(filename, ios::app);
    if (!file) {
        cerr << "Error: Cannot open file for writing.\n";
        return;
    }

    for (const auto &v : vertices) {
        file << v[0] << " " << v[1] << " " << v[2] << "\n";
    }
    file << "\n"; // Separate the sphere data
    file.close();
}

void Sphere::loadFromFile(const string &filename) {
    ifstream file(filename);
    if (!file) {
        cerr << "Error: Cannot open file for reading.\n";
        return;
    }

    vertices.clear();
    double x, y, z;
    while (file >> x >> y >> z) {
        vertices.push_back({x, y, z});
    }
    file.close();
}
