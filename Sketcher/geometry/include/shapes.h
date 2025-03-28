#ifndef SHAPES_H
#define SHAPES_H

#include <string>

class Shape {
public:
    virtual void translate(double dx, double dy, double dz) = 0;
    virtual void rotate(double angle, char axis) = 0;
    virtual void scale(double sx, double sy, double sz) = 0;
    virtual void scale(double factor) = 0;
    virtual void plot(const std::string &filename) const = 0;
    virtual void saveToFile(const std::string &filename) const = 0;
    virtual void loadFromFile(const std::string &filename) = 0;
    virtual ~Shape() {}
};

#endif
