// Scene.cpp
#include "Scene.h"

size_t Scene::addShape(const std::vector<std::vector<double>>& vertices) {
    ShapeEntry entry;
    entry.vertices = vertices;
    // No face indices for line-based shapes
    shapes.push_back(entry);
    return shapes.size() - 1; // Return the index of the newly added shape
}

size_t Scene::addShapeWithFaces(const std::vector<std::vector<double>>& vertices, 
                              const std::vector<std::array<int, 3>>& faces) {
    ShapeEntry entry;
    entry.vertices = vertices;
    entry.faceIndices = faces;
    shapes.push_back(entry);
    return shapes.size() - 1;
}

std::vector<std::vector<double>> Scene::getAllVertices() const {
    std::vector<std::vector<double>> allVertices;
    for (const auto& shape : shapes) {
        if (shape.visible) {
            for (size_t i = 0; i < shape.vertices.size(); i++) {
                allVertices.push_back(shape.vertices[i]);
            }
        }
    }
    return allVertices;
}

std::vector<std::pair<std::vector<std::vector<double>>, std::vector<std::array<int, 3>>>> Scene::getAllShapes() const {
    std::vector<std::pair<std::vector<std::vector<double>>, std::vector<std::array<int, 3>>>> result;
    for (const auto& shape : shapes) {
        if (shape.visible) {
            result.push_back(std::make_pair(shape.vertices, shape.faceIndices));
        }
    }
    return result;
}

void Scene::removeShape(size_t index) {
    if (index < shapes.size()) {
        shapes.erase(shapes.begin() + index);
    }
}

void Scene::setShapeVisibility(size_t index, bool visible) {
    if (index < shapes.size()) {
        shapes[index].visible = visible;
    }
}

size_t Scene::getShapeCount() const {
    return shapes.size();
}

const Scene::ShapeEntry& Scene::getShape(size_t index) const {
    return shapes.at(index); // Using at() for bounds checking
}

Scene::ShapeEntry& Scene::getShape(size_t index) {
    return shapes.at(index);
}