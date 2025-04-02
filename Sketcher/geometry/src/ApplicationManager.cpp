#include "ApplicationManager.h"

void ApplicationManager::run() {
    displayMenu();
}

void ApplicationManager::handleShapeCreationAndPlot(int choice) {
    auto shape = ShapeFactory::createShape(choice);
    if (shape) {
        askForTransformation(shape);
        string filenames[] = {"data/cuboid.dat", "data/cylinder.dat", "data/sphere.dat", "data/polyline.dat",
                              "data/bezier.dat", "data/triangle.dat", "data/polygon.dat", "data/line3D.dat"};
        shape->plot(filenames[choice - 1]);
    }
}

void ApplicationManager::askForTransformation(shared_ptr<Shape> shape) {
    char transformChoice;
    cout << "Do you want to apply a transformation to this shape? (y/n): ";
    cin >> transformChoice;

    if (transformChoice == 'y' || transformChoice == 'Y') {
        transformShape(shape);
    }
}

void ApplicationManager::createAndPlotHumanoid() {
    Structure humanoid;

    // Add humanoid parts
    humanoid.addShape(make_shared<Sphere>(2, 20)); // Head
    humanoid.addShape(make_shared<Cylinder>(1, 0.5, 20)); // Neck
    humanoid.addShape(make_shared<Cuboid>(4, 2, 6)); // Body
    humanoid.addShape(make_shared<Cylinder>(0.5, 4, 20)); // Left Arm
    humanoid.addShape(make_shared<Cylinder>(0.5, 4, 20)); // Right Arm
    humanoid.addShape(make_shared<Cylinder>(0.7, 5, 20)); // Left Leg
    humanoid.addShape(make_shared<Cylinder>(0.7, 5, 20)); // Right Leg


    humanoid.translate(0, 0, 8, 0); // Head
    humanoid.translate(0, 0, 5.3, 1); // Neck
    humanoid.translate(-2, -1, -0.75, 2); // Body
    humanoid.translate(-2.5, 0, 1.25, 3); // Left Arm
    humanoid.translate(2.5, 0, 1.25, 4); // Right Arm
    humanoid.translate(-1, 0, -5.5, 5); // Left Leg
    humanoid.translate(1, 0, -5.5, 6); // Right Leg

    askForHumanoidTransformation(humanoid);

    humanoid.plot("data/humanoid_combined.dat");
    cout << "Humanoid structure plotted! Check the GNUPLOT visualization.\n";
}

void ApplicationManager::askForHumanoidTransformation(Structure& humanoid) {
    char transformChoice;
    cout << "Do you want to apply a transformation to the humanoid structure? (y/n): ";
    cin >> transformChoice;

    if (transformChoice == 'y' || transformChoice == 'Y') {
        transformHumanoid(humanoid);
    }
}

void ApplicationManager::transformHumanoid(Structure& humanoid) {
    int transformOption;
    cout << "Choose a transformation:\n";
    cout << "1. Transform the entire humanoid\n";
    cout << "2. Transform an individual part\n";
    cout << "Enter your choice: ";
    cin >> transformOption;

    if (transformOption == 1) {
        applyTransformation(humanoid);
    } else if (transformOption == 2) {
        transformHumanoidPart(humanoid);
    } else {
        cout << "Invalid choice!\n";
    }
}

void ApplicationManager::applyTransformation(Structure& humanoid) {
    int operation;
    cout << "Choose a transformation:\n";
    cout << "1. Translate\n2. Rotate\n3. Scale\n";
    cout << "Enter your choice: ";
    cin >> operation;

    if (operation == 1) {
        double dx, dy, dz;
        cout << "Enter translation values (dx, dy, dz): ";
        cin >> dx >> dy >> dz;
        humanoid.translate(dx, dy, dz);
    } else if (operation == 2) {
        double angle;
        char axis;
        cout << "Enter rotation angle and axis (x, y, or z): ";
        cin >> angle >> axis;
        humanoid.rotate(angle, axis);
    } else if (operation == 3) {
        double factor;
        cout << "Enter scaling factor: ";
        cin >> factor;
        humanoid.scale(factor, factor, factor);
    } else {
        cout << "Invalid transformation option!\n";
    }
}

void ApplicationManager::transformHumanoidPart(Structure& humanoid) {
    int partIndex;
    cout << "Enter the part index to transform (1 to " << humanoid.getShapeCount() << "): ";
    cin >> partIndex;

    if (partIndex < 1 || partIndex > static_cast<int>(humanoid.getShapeCount())) {
        cout << "Invalid part index!\n";
        return;
    }

    auto part = humanoid.getShape(partIndex - 1);
    transformShape(part);
}

void ApplicationManager::transformShape(shared_ptr<Shape> shape) {
    int transformOption;
    cout << "Choose a transformation:\n";
    cout << "1. Translate\n2. Rotate\n3. Scale\n";
    cout << "Enter your choice: ";
    cin >> transformOption;

    if (transformOption == 1) {
        double dx, dy, dz;
        cout << "Enter translation values (dx, dy, dz): ";
        cin >> dx >> dy >> dz;
        shape->translate(dx, dy, dz);
    } else if (transformOption == 2) {
        double angle;
        char axis;
        cout << "Enter rotation angle and axis (x, y, or z): ";
        cin >> angle >> axis;
        shape->rotate(angle, axis);
    } else if (transformOption == 3) {
        double factor;
        cout << "Enter scaling factor: ";
        cin >> factor;
        shape->scale(factor);
    } else {
        cout << "Invalid transformation option!\n";
    }
}

void ApplicationManager::displayMenu() {
    map<int, function<void()>> menuOptions = {
        {0, [this]() { createAndPlotHumanoid(); }},
        {1, [this]() { handleShapeCreationAndPlot(1); }},
        {2, [this]() { handleShapeCreationAndPlot(2); }},
        {3, [this]() { handleShapeCreationAndPlot(3); }},
        {4, [this]() { handleShapeCreationAndPlot(4); }},
        {5, [this]() { handleShapeCreationAndPlot(5); }},
        {6, [this]() { handleShapeCreationAndPlot(6); }},
        {7, [this]() { handleShapeCreationAndPlot(7); }},
        {8, [this]() { handleShapeCreationAndPlot(8); }}
    };

    cout << "Choose an option:\n";
    cout << "0. Plot Humanoid Structure\n";
    cout << "1. Cuboid\n2. Cylinder\n3. Sphere\n4. Polyline\n5. Bezier Curve\n6. Triangle\n7. Polygon\n8. 3D Line\n";
    cout << "Enter your choice: ";
    int choice;
    cin >> choice;

    if (menuOptions.find(choice) != menuOptions.end()) {
        menuOptions[choice]();
    } else {
        cout << "Invalid choice!\n";
    }
}