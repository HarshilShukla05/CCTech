#ifndef BEZIER_H
#define BEZIER_H

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include <vector>
#include <string>
#include "shapes.h"

using namespace std;

class Bezier : public Shape {
private:
    vector<vector<double>> controlPoints;
    vector<vector<double>> curveVertices;
    int interpolationPoints = 100; // Default
    // int ip;
public:
    // Bezier(vector<vector<double>>controlPoints, int ip);
    Bezier();
    void addControlPoint(double x, double y, double z);
    //void getInterpolatedPoints(int n);
    
    void translate(double dx, double dy, double dz)override; //Using matices
    void scale(double sx, double sy, double sz)override;
    void scale(double factor) override; 
    void rotate(double angle, char axis) override;

    void plot(const std::string &filename) const override;

    vector<vector<double>> calculateBezierCurve(int numSegments) const; 
    std::vector<std::vector<double>> calculateBezierCurve() const;

    void saveToFile(const string &filename) const override;
    void loadFromFile(const string &filename) override;
    std::vector<std::vector<double>> getEdgeLines() const ;

    const std::vector<std::vector<double>>& getControlPoints() const;
    void setControlPoint(int index, const std::vector<double>& pt);


    void setInterpolationPoints(int ip) { interpolationPoints = ip; }
    int getInterpolationPoints() const { return interpolationPoints; }

    void setCurveVertices(const std::vector<std::vector<double>>& verts);
    const std::vector<std::vector<double>>& getCurveVertices() const;

};

#endif
