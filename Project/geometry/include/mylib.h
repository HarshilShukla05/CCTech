#ifndef MYLIB_H
#define MYLIB_H

#include <fstream>

class Point {
public:
    double x, y, z;
    Point(double x, double y, double z);
    void getInterpolatedPoints(std::ofstream &dataFile, double step, double distance, double dx, double dy, double dz, int numberOfPointsToInterPolate);
};

#endif
