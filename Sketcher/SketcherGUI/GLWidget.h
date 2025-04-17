#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMouseEvent>
#include <QWheelEvent>
#include <Bezier.h>

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT

private:
    std::vector<std::vector<double>> vertices;
    std::vector<std::array<int, 3>> faceIndices; // New member variable
    QPoint lastMousePosition; // For mouse interaction

    float rotationX = 0.0f;   // Rotation around X-axis
    float rotationY = 0.0f;   // Rotation around Y-axis
    float zoom = -5.0f;       // Zoom level
    QVector3D center;         // Center of the shape

    QVector3D computeCenter() const; // New method to compute center

    // For Bezier dragging
    QVector3D worldToScreen(const std::vector<double>& worldPt);

    int selectedControlPointIndex = -1;
    Bezier* bezierShape = nullptr; // Current bezier shape
    QPoint lastMousePos;


public:
    explicit GLWidget(QWidget *parent = nullptr);
    void setShapeVertices(const std::vector<std::vector<double>> &verts); // For normal shapes
    std::vector<std::vector<double>> getShapeVertices() const;
    void setShapeData(const std::vector<std::vector<double>> &verts, 
                      const std::vector<std::array<int, 3>> &faces); // New method
    void setBezierShape(Bezier* bezier);


protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    
};