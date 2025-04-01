#include "cuboid.h"
#include "plot_utils.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>

using namespace std;

Cuboid::Cuboid(double length, double width, double height) {
    vertices = {
        {0, 0, 0}, {length, 0, 0}, {length, width, 0}, {0, width, 0},
        {0, 0, height}, {length, 0, height}, {length, width, height}, {0, width, height}
    };
}

void Cuboid::saveToFile(const string &filename) const {
    ofstream file(filename, ios::trunc);
    if (!file) {
        cerr << "Error: Cannot open file for writing.\n";
        return;
    }

    // Define edges between vertices
    int edges[12][2] = {
        {0, 1}, {1, 2}, {2, 3}, {3, 0}, // Bottom face
        {4, 5}, {5, 6}, {6, 7}, {7, 4}, // Top face
        {0, 4}, {1, 5}, {2, 6}, {3, 7}  // Vertical edges
    };

    for (int i = 0; i < 12; i++) {
        int v1 = edges[i][0], v2 = edges[i][1];
        file << vertices[v1][0] << " " << vertices[v1][1] << " " << vertices[v1][2] << "\n";
        file << vertices[v2][0] << " " << vertices[v2][1] << " " << vertices[v2][2] << "\n";
        file << "\n"; 
        file << "e\n"; 
    }

    file.close();
}

void Cuboid::translate(double dx, double dy, double dz) {
    PlotUtils::translate(vertices, dx, dy, dz);
}

void Cuboid::scale(double sx, double sy, double sz) {
    PlotUtils::scale(vertices, sx, sy, sz);
}

void Cuboid::scale(double factor) {
    scale(factor, factor, factor);
}

void Cuboid::rotate(double angle, char axis) {
    PlotUtils::rotate(vertices, angle, axis);
}

void Cuboid::loadFromFile(const string &filename) {
    ifstream file(filename);
    vertices.clear();
    double x, y, z;
    while (file >> x >> y >> z) {
        vertices.push_back({x, y, z});
    }
    file.close();
}

void Cuboid::plot(const string &filename) const {
    saveToFile(filename);
    PlotUtils::plot(filename, "Cuboid", "black");
}
