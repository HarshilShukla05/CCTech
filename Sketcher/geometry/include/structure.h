#ifndef STRUCTURE_H
#define STRUCTURE_H

#include <vector>
#include <memory>
#include "shapes.h"

using namespace std;

class Structure {
private:
    vector<shared_ptr<Shape>> shapes;  // Stores all shapes

public:
    void addShape(shared_ptr<Shape> shape);
    void translate(double dx, double dy, double dz, int shapeIndex = -1);
    void scale(double sx, double sy, double sz, int shapeIndex = -1);
    void rotate(double angle, char axis, int shapeIndex = -1);
    void plot(const string &filename);

    void saveToFile(const string &filename);
    void loadFromFile(const string &filename);

    // methods to access shapes
    size_t getShapeCount() const;
    shared_ptr<Shape> getShape(int index) const;

    void listShapes() const;
};

#endif 
