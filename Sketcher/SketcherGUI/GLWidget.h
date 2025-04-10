#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions>

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT

private:
    std::vector<std::vector<double>> vertices;

public:
    explicit GLWidget(QWidget *parent = nullptr);
    void setShapeVertices(const std::vector<std::vector<double>> &verts);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
};