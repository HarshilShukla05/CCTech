#ifndef APPLICATION_MANAGER_H
#define APPLICATION_MANAGER_H

#include <map>
#include <functional>
#include <memory>
#include <iostream>
#include <string>
#include "structure.h"
#include "ShapeFactory.h"

using namespace std;

class ApplicationManager {
public:
    void run();

private:
    void handleShapeCreationAndPlot(int choice);
    void displayMenu();

    void createAndPlotHumanoid();
    void transformHumanoid(Structure& humanoid);
    void applyTransformation(Structure& humanoid);
    void transformHumanoidPart(Structure& humanoid);

    void transformShape(shared_ptr<Shape> shape);
    void askForTransformation(shared_ptr<Shape> shape);
    void askForHumanoidTransformation(Structure& humanoid);
};

#endif
