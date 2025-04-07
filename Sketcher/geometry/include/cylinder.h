#ifndef CYLINDER_H
#define CYLINDER_H

#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include "shapes.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

using namespace std;

class Cylinder : public Shape {
private:
    double radius, height;
    int resolution; // Number of points in the circular base
    vector<vector<double>> vertices;

public:
    Cylinder(double r, double h, int res);
    
    void generateVertices();
    void saveToFile(const string &filename) const override;
    void loadFromFile(const string &filename) override;
    
    void translate(double dx, double dy, double dz)override;
    void scale(double sx, double sy, double sz)override;
    void scale(double factor) override; 
    void rotate(double angle, char axis) override;

    void plot(const std::string &filename) const override;
};

#endif
