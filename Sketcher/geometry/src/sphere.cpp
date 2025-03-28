#include "sphere.h"
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

void Sphere::plot(const string& filename) const {
    ofstream file(filename);
    for (const auto& v : vertices) {
        file << v[0] << " " << v[1] << " " << v[2] << "\n";
    }
    file.close();

    ofstream gnuplotFile("plot_sphere.gnu");
    gnuplotFile << "set terminal wxt\n";
    gnuplotFile << "set parametric\n";
    gnuplotFile << "set hidden3d\n";
    gnuplotFile << "set mouse\n";
    gnuplotFile << "set urange [0:2*pi]\n";
    gnuplotFile << "set vrange [0:pi]\n";
    gnuplotFile << "set xlabel 'X'\n";
    gnuplotFile << "set ylabel 'Y'\n";
    gnuplotFile << "set zlabel 'Z'\n";
    gnuplotFile << "set view equal xyz\n";
    gnuplotFile << "r = " << radius << "\n";
    gnuplotFile << "splot r*sin(v)*cos(u), r*sin(v)*sin(u), r*cos(v) with lines\n";
    gnuplotFile.close();

    system("gnuplot -p plot_sphere.gnu");
}

void Sphere::translate(double dx, double dy, double dz) {
    for (auto& v : vertices) {
        v[0] += dx;
        v[1] += dy;
        v[2] += dz;
    }
}

void Sphere::scale(double sx, double sy, double sz) {
    for (auto& v : vertices) {
        v[0] *= sx;
        v[1] *= sy;
        v[2] *= sz;
    }
}

void Sphere::scale(double factor) {
    scale(factor, factor, factor); // Delegate to multi-factor scaling
}

void Sphere::rotate(double angle, char axis) {
    // Rotation does not affect the geometry of a sphere
    // This implemented to satisfy the Shape interface
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
