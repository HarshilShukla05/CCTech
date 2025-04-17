// Scene.h
#pragma once

#include <vector>
#include <memory>
#include <array>
#include <vector>
#include <QVector3D>
#include "shapes.h"

class Scene {
private:
    struct ShapeEntry {
        std::vector<std::vector<double>> vertices;
        std::vector<std::array<int, 3>> faceIndices;
        bool visible = true;
        // You could add more properties like color, name, etc.
    };

    std::vector<ShapeEntry> shapes;

public:
    Scene() = default;
    
    // Add a shape defined by lines (like your basic shapes)
    size_t addShape(const std::vector<std::vector<double>>& vertices);
    
    // Add a shape defined by triangulated faces (like OBJ/STL files)
    size_t addShapeWithFaces(const std::vector<std::vector<double>>& vertices, 
                           const std::vector<std::array<int, 3>>& faces);
    
    // Get all shape data for rendering
    std::vector<std::vector<double>> getAllVertices() const;
    std::vector<std::pair<std::vector<std::vector<double>>, std::vector<std::array<int, 3>>>> getAllShapes() const;
    
    // Manipulate individual shapes
    void removeShape(size_t index);
    void setShapeVisibility(size_t index, bool visible);
    size_t getShapeCount() const;
    
    // Get a specific shape
    const ShapeEntry& getShape(size_t index) const;
    ShapeEntry& getShape(size_t index);
};