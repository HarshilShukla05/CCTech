#ifndef CUBOID_H
#define CUBOID_H

#include <vector>
#include <string>
#include "shapes.h"

using namespace std;

class Cuboid : public Shape {
private:
    vector<vector<double>> vertices; // Stores cuboid vertices
public:
    Cuboid(double length, double width, double height);
    
    void translate(double dx, double dy, double dz)override;
    void scale(double sx, double sy, double sz)override;
    void scale(double factor) override; 
    void rotate(double angle, char axis) override;

    void saveToFile(const string &filename) const; 
    void loadFromFile(const string &filename);

    void plot(const std::string &filename) const override;
    std::vector<std::vector<double>> getEdgeLines() const;
    // const vector<vector<double>>& getVertices() const { return vertices; }

};

#endif // CUBOID_H