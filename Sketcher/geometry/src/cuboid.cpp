#include "cuboid.h"
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
    ofstream file(filename, ios::app);
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
        file << "\n"; // Separate each edge
    }

    file.close();
}

void Cuboid::translate(double dx, double dy, double dz) {
    for (auto &v : vertices) {
        v[0] += dx;
        v[1] += dy;
        v[2] += dz;
    }
}

void Cuboid::scale(double sx, double sy, double sz) {
    for (auto &v : vertices) {
        v[0] *= sx;
        v[1] *= sy;
        v[2] *= sz;
    }
}

void Cuboid::scale(double factor) {
    scale(factor, factor, factor); // Delegate to multi-factor scaling
}

void Cuboid::rotate(double angle, char axis) {
    double rad = angle * M_PI / 180.0;
    for (auto &v : vertices) {
        double x = v[0], y = v[1], z = v[2];
        if (axis == 'x') {
            v[1] = y * cos(rad) - z * sin(rad);
            v[2] = y * sin(rad) + z * cos(rad);
        } else if (axis == 'y') {
            v[0] = x * cos(rad) + z * sin(rad);
            v[2] = -x * sin(rad) + z * cos(rad);
        } else if (axis == 'z') {
            v[0] = x * cos(rad) - y * sin(rad);
            v[1] = x * sin(rad) + y * cos(rad);
        }
    }
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

    ofstream gnuplot("plot.gnu");
    if (!gnuplot) {
        cerr << "Error: Cannot open GNUPlot script.\n";
        return;
    }

    gnuplot << "splot '" << filename << "' with linespoints pointtype 7 linecolor 'black' linewidth 2\n";
    gnuplot << "pause -1\n";
    gnuplot.close();

    system("gnuplot plot.gnu");
}
