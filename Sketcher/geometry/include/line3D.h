#ifndef LINE3D_H
#define LINE3D_H

#include <vector>
#include <string>
#include "shapes.h"

using namespace std;

class Line3D : public Shape {
private:
    vector<vector<double>> points; // Stores two points (x, y, z)

public:
    Line3D(); 

    void setPoints(double x1, double y1, double z1, double x2, double y2, double z2);
    void plot(const std::string &filename) const override;

    void translate(double dx, double dy, double dz)override;
    void scale(double sx, double sy, double sz)override;
    void scale(double factor) override; 
    void rotate(double angle, char axis) override;
    void saveToFile(const string &filename) const override;
    void loadFromFile(const string &filename) override;
};

#endif 
