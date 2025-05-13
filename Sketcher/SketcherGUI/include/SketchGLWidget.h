#ifndef SKETCHGLWIDGET_H
#define SKETCHGLWIDGET_H

#include <QOpenGLWidget>
#include <QMouseEvent>
#include <vector>
#include <QPointF>

class SketchGLWidget : public QOpenGLWidget {
    Q_OBJECT

public:
    explicit SketchGLWidget(QWidget* parent = nullptr);

    void finishCurrentShape();
    void applyUnion();
    void applyIntersection();
    void applySubtraction(bool isAB);

    std::vector<std::vector<QPointF>> getPolygons() const;

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void mousePressEvent(QMouseEvent* event) override;

private:
    std::vector<QPointF> currentPoints;
    std::vector<std::vector<QPointF>> shapes;
};

#endif // SKETCHGLWIDGET_H
