#ifndef SPHERE_H
#define SPHERE_H

#include <vector>
#include <string>
#include "shapes.h"

using namespace std;

class Sphere : public Shape {
private:
    double radius;
    int segments;
    vector<vector<double>> vertices;

public:
    Sphere(double r, int seg);
    void plot(const std::string &filename) const override;
    void translate(double dx, double dy, double dz)override;
    void scale(double sx, double sy, double sz) override; 
    void scale(double factor) override; 
    void rotate(double angle, char axis) override; 
    void saveToFile(const string &filename) const override;
    void loadFromFile(const string &filename) override;
    std::vector<std::vector<double>> getEdgeLines()  const;

};

#endif
