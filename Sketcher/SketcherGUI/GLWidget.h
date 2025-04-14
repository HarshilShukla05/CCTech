#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMouseEvent>
#include <QWheelEvent>

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT

private:
    std::vector<std::vector<double>> vertices;

    float rotationX = 0.0f;
    float rotationY = 0.0f;
    float zoom = -100.0f;

    QPoint lastMousePos;
    QVector3D computeCenter() const;

public:
    explicit GLWidget(QWidget *parent = nullptr);
    void setShapeVertices(const std::vector<std::vector<double>> &verts);
    std::vector<std::vector<double>> getShapeVertices() const;

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    
};
