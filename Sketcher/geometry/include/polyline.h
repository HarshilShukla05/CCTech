#ifndef POLYLINE_H
#define POLYLINE_H

#include <vector>
#include <string>
#include "shapes.h"

using namespace std;

class Polyline : public Shape {
private:
    vector<vector<double>> points; // Stores (x, y, z) coordinates

public:
    Polyline();  

    void addPoint(double x, double y, double z); 
    void plot(const std::string &filename) const override;
    void translate(double dx, double dy, double dz)override;
    void scale(double sx, double sy, double sz)override; 
    void scale(double factor) override; 
    void rotate(double angle, char axis) override;  

    void saveToFile(const string& filename) const;  
    void loadFromFile(const string& filename); 
    std::vector<std::vector<double>> getEdgeLines() const; // Corrected return type
};

#endif
