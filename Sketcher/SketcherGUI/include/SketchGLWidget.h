#ifndef SKETCHGLWIDGET_H
#define SKETCHGLWIDGET_H

#include <QOpenGLWidget>
#include <QMouseEvent>
#include <QColor>
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
    
    // int getSelectedRegionIndex() const { return selectedRegionIndex; }
    // std::vector<QPointF> getSelectedPolygon() const;

    // Method to clear the result
    void clearResult();

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void mousePressEvent(QMouseEvent* event) override;

private:
    std::vector<QPointF> currentPoints;
    std::vector<std::vector<QPointF>> shapes;

    // Store the shaded region for visualization
    std::vector<QPointF> shadedRegion;

    // Store the color of the result
    QColor resultColor;
};

#endif // SKETCHGLWIDGET_H
