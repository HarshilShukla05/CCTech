#ifndef POLYGON_H
#define POLYGON_H

#include <vector>
#include <string>
#include "shapes.h" // Assuming Shape class is defined here

using namespace std;

class Polygon : public Shape {
private:
    vector<vector<double>> vertices; // Stores (x, y, z) coordinates

    // Helper function to calculate the centroid of the polygon
    void calculateCentroid(double &cx, double &cy, double &cz);

    // Helper method to generate the vertices for a regular polygon
    void generateRegularPolygon(int sides, double radius);

public:
    // Constructor for a regular polygon with a given number of sides and radius
    Polygon(int sides, double radius); 

    // Method to add a vertex to the polygon
    void addVertex(double x, double y, double z);

    // Method to plot the polygon (save to file and plot it)
    void plot(const std::string &filename) const override;

    // Transformation methods
    void translate(double dx, double dy, double dz) override;
    void scale(double sx, double sy, double sz) override;
    void scale(double factor) override;  // Scale uniformly
    void rotate(double angle, char axis) override;

    // Methods for file operations
    void saveToFile(const string &filename) const override;
    void loadFromFile(const string &filename) override;

    // Method to get the edges for plotting or rendering
    std::vector<std::vector<double>> getEdgeLines() const;
};

#endif
