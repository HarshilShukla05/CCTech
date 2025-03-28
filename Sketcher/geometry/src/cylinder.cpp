#include "cylinder.h"

using namespace std;

Cylinder::Cylinder(double r, double h, int res) : radius(r), height(h), resolution(res) {
    generateVertices();
}

void Cylinder::generateVertices() {
    double angleStep = 2 * M_PI / resolution;

    // Bottom circle
    for (int i = 0; i < resolution; i++) {
        double x = radius * cos(i * angleStep);
        double y = radius * sin(i * angleStep);
        vertices.push_back({x, y, 0});
    }

    // Top circle
    for (int i = 0; i < resolution; i++) {
        double x = radius * cos(i * angleStep);
        double y = radius * sin(i * angleStep);
        vertices.push_back({x, y, height});
    }
}

void Cylinder::saveToFile(const string &filename) const {
    ofstream file(filename, ios::app);
    if (!file) {
        cerr << "Error: Cannot open file for writing.\n";
        return;
    }

    // Write bottom circle
    for (int i = 0; i < resolution; i++) {
        file << vertices[i][0] << " " << vertices[i][1] << " " << vertices[i][2] << "\n";
        file << vertices[(i + 1) % resolution][0] << " " << vertices[(i + 1) % resolution][1] << " " << vertices[(i + 1) % resolution][2] << "\n\n";
    }

    // Write top circle
    for (int i = resolution; i < 2 * resolution; i++) {
        file << vertices[i][0] << " " << vertices[i][1] << " " << vertices[i][2] << "\n";
        file << vertices[(i + 1) % resolution + resolution][0] << " " << vertices[(i + 1) % resolution + resolution][1] << " " << vertices[(i + 1) % resolution + resolution][2] << "\n\n";
    }

    // Write vertical edges
    for (int i = 0; i < resolution; i++) {
        file << vertices[i][0] << " " << vertices[i][1] << " " << vertices[i][2] << "\n";
        file << vertices[i + resolution][0] << " " << vertices[i + resolution][1] << " " << vertices[i + resolution][2] << "\n\n";
    }

    file.close();
}

void Cylinder::loadFromFile(const string &filename) {
    // Implementation for loading cylinder data
}

void Cylinder::plot(const string &filename) const {
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

void Cylinder::translate(double dx, double dy, double dz) {
    for (auto &v : vertices) {
        v[0] += dx;
        v[1] += dy;
        v[2] += dz;
    }
}

void Cylinder::rotate(double angle, char axis) {
    double radians = angle * M_PI / 180.0;

    for (auto& v : vertices) {
        double x = v[0], y = v[1], z = v[2];

        if (axis == 'x') {
            v[1] = y * cos(radians) - z * sin(radians);
            v[2] = y * sin(radians) + z * cos(radians);
        } else if (axis == 'y') {
            v[0] = x * cos(radians) + z * sin(radians);
            v[2] = -x * sin(radians) + z * cos(radians);
        } else if (axis == 'z') {
            v[0] = x * cos(radians) - y * sin(radians);
            v[1] = x * sin(radians) + y * cos(radians);
        }
    }
}

void Cylinder::scale(double sx, double sy, double sz) {
    for (auto& v : vertices) {
        v[0] *= sx;
        v[1] *= sy;
        v[2] *= sz;
    }
}

void Cylinder::scale(double factor) {
    scale(factor, factor, factor); // Delegate to multi-factor scaling
}