#include "triangle.h"
#include "plot_utils.h"

using namespace std;

Triangle::Triangle(double x1, double y1, double z1,
                   double x2, double y2, double z2,
                   double x3, double y3, double z3) {
    vertices = {{x1, y1, z1}, {x2, y2, z2}, {x3, y3, z3}};
}

void Triangle::translate(double dx, double dy, double dz) {
    PlotUtils::translate(vertices, dx, dy, dz);
}

void Triangle::scale(double sx, double sy, double sz) {
    PlotUtils::scale(vertices, sx, sy, sz);
}

void Triangle::scale(double factor) {
    scale(factor, factor, factor);
}

void Triangle::rotate(double angle, char axis) {
    double cx = 0, cy = 0, cz = 0;
    for (const auto& v : vertices) {
        cx += v[0];
        cy += v[1];
        cz += v[2];
    }
    cx /= vertices.size();
    cy /= vertices.size();
    cz /= vertices.size();

    PlotUtils::rotate(vertices, angle, axis, cx, cy, cz);
}

void Triangle::plot(const string& filename) const {
    saveToFile(filename);
    PlotUtils::plot(filename, "Triangle", "red");
}

void Triangle::saveToFile(const string &filename) const {
    ofstream file(filename, ios::app);
    if (!file) {
        cerr << "Error: Cannot open file for writing.\n";
        return;
    }
    file << "Triangle\n";
    for (const auto &v : vertices) {
        file << v[0] << " " << v[1] << " " << v[2] << "\n";
    }
    file.close();
}

void Triangle::loadFromFile(const string &filename) {
    ifstream file(filename);
    if (!file) {
        cerr << "Error: Cannot open file for reading.\n";
        return;
    }
    vertices.clear();
    for (int i = 0; i < 3; ++i) {
        double x, y, z;
        file >> x >> y >> z;
        vertices.push_back({x, y, z});
    }
    file.close();
}