#ifndef SKETCHWINDOW_H
#define SKETCHWINDOW_H

#include <QMainWindow>

class SketchGLWidget;

class SketchWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit SketchWindow(QWidget* parent = nullptr);
    void onStlIntersection(); // Move this line here from private section

private slots:
    void onFinishShape();
    void onUnion();
    void onIntersection();
    void onSubtraction();
    void onTriangleIntersection();

private:
    SketchGLWidget* sketchGLWidget; // Corrected variable name
    QString stlFileA, stlFileB; // Store STL file paths
    void loadStlFilesAndIntersect(); // Helper
};

#endif // SKETCHWINDOW_H
