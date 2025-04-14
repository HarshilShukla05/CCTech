#include "polyline.h"
#include "plot_utils.h"
#include <iostream>
#include <fstream>

using namespace std;

Polyline::Polyline() {}

void Polyline::addPoint(double x, double y, double z) {
    points.push_back({x, y, z});
}

void Polyline::translate(double dx, double dy, double dz) {
    PlotUtils::translate(points, dx, dy, dz);
}

void Polyline::scale(double sx, double sy, double sz) {
    PlotUtils::scale(points, sx, sy, sz);
}

void Polyline::scale(double factor) {
    scale(factor, factor, factor);
}

void Polyline::rotate(double angle, char axis) {
    PlotUtils::rotate(points, angle, axis);
}

void Polyline::saveToFile(const string& filename) const {
    ofstream file(filename);
    if (!file) {
        cerr << "Error: Cannot save file " << filename << endl;
        return;
    }
    for (const auto& point : points) {
        file << point[0] << " " << point[1] << " " << point[2] << "\n";
    }
    file.close();
}

void Polyline::loadFromFile(const string& filename) {
    ifstream file(filename);
    if (!file) {
        cerr << "Error: Cannot open file " << filename << endl;
        return;
    }

    points.clear();
    double x, y, z;
    while (file >> x >> y >> z) {
        points.push_back({x, y, z});
    }
    file.close();
}

void Polyline::plot(const string& filename) const {
    saveToFile(filename);
    PlotUtils::plot(filename, "Polyline", "green");
}

std::vector<std::vector<double>> Polyline::getEdgeLines() const {
    std::vector<std::vector<double>> lines;
    for (size_t i = 0; i < points.size() - 1; ++i) {
        lines.push_back(points[i]);
        lines.push_back(points[i + 1]);
    }
    return lines;
}