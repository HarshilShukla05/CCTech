#ifndef SHAPE_FACTORY_H
#define SHAPE_FACTORY_H

#include <memory>
#include <map>
#include <functional>
#include <iostream>
#include "cuboid.h"
#include "cylinder.h"
#include "sphere.h"
#include "polyline.h"
#include "bezier.h"
#include "triangle.h"
#include "polygon.h"
#include "line3D.h"

using namespace std;

class ShapeFactory {
public:
    static shared_ptr<Shape> createShape(int choice);
};

#endif
