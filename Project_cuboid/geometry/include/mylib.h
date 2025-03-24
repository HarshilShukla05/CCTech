#ifndef MYLIB_H
#define MYLIB_H

#include <fstream>

class Point {
public:
    double x, y, z;
    Point(double x, double y, double z);
    void getInterpolatedPoints(std::ofstream &dataFile, double step, double distance, double dx, double dy, double dz, int numberOfPointsToInterPolate);
};

// New Cuboid Class
class Cuboid {
public:
    double x, y, z, length, breadth, height;
    Cuboid(double x, double y, double z, double l, double b, double h);
    void writeCuboidData(std::ofstream &dataFile);
};

#endif
