#include "cylinder.h"
#include "plot_utils.h"

using namespace std;

Cylinder::Cylinder(double r, double h, int res) : radius(r), height(h), resolution(res) {
    generateVertices();
}

void Cylinder::generateVertices() {
    double angleStep = 2 * M_PI / resolution;

    // Bottom 
    for (int i = 0; i < resolution; i++) {
        double x = radius * cos(i * angleStep);
        double y = radius * sin(i * angleStep);
        vertices.push_back({x, y, 0});
    }

    // Top 
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

    for (int i = 0; i < resolution; i++) {
        file << vertices[i][0] << " " << vertices[i][1] << " " << vertices[i][2] << "\n";
        file << vertices[(i + 1) % resolution][0] << " " << vertices[(i + 1) % resolution][1] << " " << vertices[(i + 1) % resolution][2] << "\n\n";
    }

    for (int i = resolution; i < 2 * resolution; i++) {
        file << vertices[i][0] << " " << vertices[i][1] << " " << vertices[i][2] << "\n";
        file << vertices[(i + 1) % resolution + resolution][0] << " " << vertices[(i + 1) % resolution + resolution][1] << " " << vertices[(i + 1) % resolution + resolution][2] << "\n\n";
    }

    for (int i = 0; i < resolution; i++) {
        file << vertices[i][0] << " " << vertices[i][1] << " " << vertices[i][2] << "\n";
        file << vertices[i + resolution][0] << " " << vertices[i + resolution][1] << " " << vertices[i + resolution][2] << "\n\n";
    }

    file.close();
}

void Cylinder::loadFromFile(const string &filename) {
    
}

void Cylinder::plot(const string& filename) const {
    saveToFile(filename);
    PlotUtils::plot(filename, "Cylinder", "black");
}

void Cylinder::translate(double dx, double dy, double dz) {
    PlotUtils::translate(vertices, dx, dy, dz);
}

void Cylinder::rotate(double angle, char axis) {
    PlotUtils::rotate(vertices, angle, axis);
}

void Cylinder::scale(double sx, double sy, double sz) {
    PlotUtils::scale(vertices, sx, sy, sz);
}

void Cylinder::scale(double factor) {
    scale(factor, factor, factor);
}