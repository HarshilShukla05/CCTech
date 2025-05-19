#ifndef SKETCHGLWIDGET_H
#define SKETCHGLWIDGET_H

#include <QOpenGLWidget>
#include <QMouseEvent>
#include <QColor>
#include <QVector3D>
#include <vector>
#include <QPointF>
#include <array>

class SketchGLWidget : public QOpenGLWidget {
    Q_OBJECT

public:
    explicit SketchGLWidget(QWidget* parent = nullptr);

    void finishCurrentShape();
    void applyUnion();
    void applyIntersection();
    void applySubtraction(bool isAB);

    std::vector<std::vector<QPointF>> getPolygons() const;
    
    int getSelectedRegionIndex() const { return selectedRegionIndex; }
    std::vector<QPointF> getSelectedPolygon() const;

    void setExtrudedData(const std::vector<QVector3D>& verts, const std::vector<unsigned int>& indices); 
    void clearResult();
    void extrudeSelectedRegion(float depth);
    
    void toggleBezierMode();
    void findBezierIntersections();

    bool isBezierModeActive() const;

    void setResultRegion(const std::vector<QPointF>& pts);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    


private:
    struct Geometry {
        std::vector<QVector3D> vertices;
        std::vector<unsigned int> indices;
    };

    Geometry extrudedGeometry; // Store extruded geometry
    
    enum class DrawMode { Line, Bezier } drawMode;

    struct CurveSegment {
        enum Type { Line, CubicBezier } type;
        QPointF p0, p1, p2, p3;
    };

    std::vector<std::vector<CurveSegment>> shapeCurves;
    std::vector<QPointF> bezierControlPoints;
    std::vector<QPointF> bezierIntersections;

    void drawGeometry();
    void drawExtrudedGeometry();

    int selectedRegionIndex = -1;
    std::vector<QPointF> currentPoints;
    std::vector<std::vector<QPointF>> shapes;
    std::vector<std::vector<QPointF>> resultRegions;
    // Store the shaded region for visualization
    std::vector<QPointF> shadedRegion;

    // Store the color of the result
    QColor resultColor;

    std::vector<QVector3D> extrudedVertices; // Added member variable
    std::vector<unsigned int> extrudedIndices; // Added member variable
    std::vector<std::array<int, 3>> extrudedFaces; // Added member variable

    bool bezierMode = false;
    void drawBezierCurve(const CurveSegment& bezier, QPainter& painter);
    std::vector<QPointF> flattenBezier(const CurveSegment& bez, int segments);
    void intersectBezierRecursive(const CurveSegment& a, const CurveSegment& b, std::vector<QPointF>& intersections, int depth = 0);

    int hoveredIntersectionIndex = -1;

};

// bool pointInPolygon(const QPointF& point, const std::vector<QPointF>& polygon);
bool pointFuzzyEqual(const QPointF& a, const QPointF& b, double eps = 1e-3);

#endif // SKETCHGLWIDGET_H
