#include "line3D.h"
#include "plot_utils.h"
#include <iostream> 
#include <fstream> 

using namespace std;

Line3D::Line3D() {
    points.resize(2, vector<double>(3, 0.0));
}

void Line3D::setPoints(double x1, double y1, double z1, double x2, double y2, double z2) {
    points[0] = {x1, y1, z1};
    points[1] = {x2, y2, z2};
}

void Line3D::translate(double dx, double dy, double dz) {
    PlotUtils::translate(points, dx, dy, dz);
}

void Line3D::scale(double sx, double sy, double sz) {
    PlotUtils::scale(points, sx, sy, sz);
}

void Line3D::scale(double factor) {
    scale(factor, factor, factor);
}

void Line3D::rotate(double angle, char axis) {
    double cx = (points[0][0] + points[1][0]) / 2.0;
    double cy = (points[0][1] + points[1][1]) / 2.0;
    double cz = (points[0][2] + points[1][2]) / 2.0;
    PlotUtils::rotate(points, angle, axis, cx, cy, cz);
}

void Line3D::plot(const string& filename) const {
    saveToFile(filename);
    PlotUtils::plot(filename, "Line3D", "blue");
}

void Line3D::saveToFile(const string &filename) const {
    ofstream file(filename, ios::app);
    if (!file) {
        cerr << "Error: Cannot open file for writing.\n";
        return;
    }
    file << "Line3D\n";
    for (const auto &p : points) {
        file << p[0] << " " << p[1] << " " << p[2] << "\n";
    }
    file.close();
}

void Line3D::loadFromFile(const string &filename) {
    ifstream file(filename);
    if (!file) {
        cerr << "Error: Cannot open file for reading.\n";
        return;
    }
    points.clear();
    for (int i = 0; i < 2; ++i) {
        double x, y, z;
        file >> x >> y >> z;
        points.push_back({x, y, z});
    }
    file.close();
}

std::vector<std::vector<double>> Line3D::getEdgeLines() const {
    // Return the points that define the line as the "edge lines"
    return points;
}