#ifndef PLOT_UTILS_H
#define PLOT_UTILS_H

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


#include <string>
#include <vector>

using namespace std;

typedef vector<vector<double>> Matrix; 

namespace PlotUtils {
    void plot(const string &filename, const string &title, const string &color = "blue");
    void translate(Matrix &vertices, double dx, double dy, double dz);
    void scale(Matrix &vertices, double sx, double sy, double sz);
    void rotate(Matrix &vertices, double angle, char axis, double cx = 0, double cy = 0, double cz = 0);

    void applyMatrix(Matrix &vertices, const Matrix &matrix);
    Matrix createTranslationMatrix(double dx, double dy, double dz);
    Matrix createScalingMatrix(double sx, double sy, double sz);
    Matrix createRotationMatrix(double angle, char axis);
}

#endif
