#include "polyline.h"
#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

Polyline::Polyline() {}

void Polyline::addPoint(double x, double y, double z) {
    points.push_back({x, y, z});
}

void Polyline::translate(double dx, double dy, double dz) {
    for (auto& point : points) {
        point[0] += dx;
        point[1] += dy;
        point[2] += dz;
    }
}

void Polyline::scale(double sx, double sy, double sz) {
    for (auto& point : points) {
        point[0] *= sx;
        point[1] *= sy;
        point[2] *= sz;
    }
}

void Polyline::scale(double factor) {
    scale(factor, factor, factor); // Delegate to multi-factor scaling
}

void Polyline::rotate(double angle, char axis) {
    double radians = angle * M_PI / 180.0;
    for (auto& point : points) {
        double x = point[0], y = point[1], z = point[2];

        if (axis == 'x') {
            point[1] = y * cos(radians) - z * sin(radians);
            point[2] = y * sin(radians) + z * cos(radians);
        } else if (axis == 'y') {
            point[0] = x * cos(radians) + z * sin(radians);
            point[2] = -x * sin(radians) + z * cos(radians);
        } else if (axis == 'z') {
            point[0] = x * cos(radians) - y * sin(radians);
            point[1] = x * sin(radians) + y * cos(radians);
        }
    }
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
    ofstream file(filename);
    if (!file) {
        cerr << "Error: Unable to create file: " << filename << endl;
        return;
    }

    for (const auto& point : points) {
        file << point[0] << " " << point[1] << " " << point[2] << endl;
    }
    file << "\n"; 
    file.close();

    string command = "gnuplot -p -e \"splot '" + filename + "' with linespoints title 'Polyline'; pause -1\"";
    int result = system(command.c_str());

    if (result != 0) {
        cerr << "Error: Could not execute GNUPlot command." << endl;
    }
}