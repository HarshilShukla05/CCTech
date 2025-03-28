#ifndef BEZIER_H
#define BEZIER_H

#include <vector>
#include <string>
#include "shapes.h"

using namespace std;

class Bezier : public Shape {
private:
    vector<vector<double>> controlPoints;

public:
    Bezier();
    void addControlPoint(double x, double y, double z);
    
    void translate(double dx, double dy, double dz)override; //Using matices
    void scale(double sx, double sy, double sz)override;
    void scale(double factor) override; // Single-factor scaling
    void rotate(double angle, char axis) override;

    void plot(const std::string &filename) const override;

    vector<vector<double>> calculateBezierCurve(int numSegments) const; // Marked as const

    void saveToFile(const string &filename) const override;
    void loadFromFile(const string &filename) override;
};

#endif
