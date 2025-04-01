#ifndef SHAPES_H
#define SHAPES_H

#include <string>
#include <vector>
#include "plot_utils.h"

using namespace std;

class Shape {
protected:
    vector<vector<double>> vertices; 

public:
    virtual void saveToFile(const string &filename) const = 0;
    virtual void loadFromFile(const string &filename) = 0;

    virtual void plot(const string &filename) const {
        saveToFile(filename);
        PlotUtils::plot(filename, "Shape");
    }

    virtual void translate(double dx, double dy, double dz) {
        PlotUtils::translate(vertices, dx, dy, dz);
    }

    virtual void scale(double sx, double sy, double sz) {
        PlotUtils::scale(vertices, sx, sy, sz);
    }

    virtual void scale(double factor) {
        scale(factor, factor, factor);
    }

    virtual void rotate(double angle, char axis) {
        PlotUtils::rotate(vertices, angle, axis);
    }

    virtual ~Shape() {}
};

#endif
