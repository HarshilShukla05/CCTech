#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>
#include "shapes.h"

using namespace std;

class Triangle : public Shape {
private:
    vector<vector<double>> vertices; // 3 vertices, each with (x, y, z)

public:
    Triangle(double x1, double y1, double z1,
             double x2, double y2, double z2,
             double x3, double y3, double z3);

    void translate(double dx, double dy, double dz)override;
    void scale(double sx, double sy, double sz)override;
    void scale(double factor) override; // Added single-factor scaling
    void rotate(double angle, char axis) override; // Added to match Shape interface
    void rotate(double angle, char axis, double Px, double Py, double Pz); // Existing method
    void plot(const std::string &filename) const override;
    void saveToFile(const string &filename) const override;
    void loadFromFile(const string &filename) override;
};

#endif
