#ifndef POLYGON_H
#define POLYGON_H

#include <vector>
#include <string>
#include "shapes.h"

using namespace std;

class Polygon : public Shape {

private:
    vector<vector<double>> vertices; // Stores (x, y, z) coordinates
    void calculateCentroid(double &cx, double &cy, double &cz); 
public:
    Polygon(int sides); 

    void addVertex(double x, double y, double z);
    void plot(const std::string &filename) const override;

    void translate(double dx, double dy, double dz) override;
    void scale(double sx, double sy, double sz) override;
    void scale(double factor) override; 
    void rotate(double angle, char axis) override;
    void saveToFile(const string &filename) const override;
    void loadFromFile(const string &filename) override;
};

#endif 
