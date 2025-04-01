#ifndef PLOT_UTILS_H
#define PLOT_UTILS_H

#include <string>
#include <vector>

using namespace std;

namespace PlotUtils {
    void plot(const string &filename, const string &title, const string &color = "blue");
    void translate(vector<vector<double>> &vertices, double dx, double dy, double dz);
    void scale(vector<vector<double>> &vertices, double sx, double sy, double sz);
    void rotate(vector<vector<double>> &vertices, double angle, char axis, double cx = 0, double cy = 0, double cz = 0);
}

#endif
