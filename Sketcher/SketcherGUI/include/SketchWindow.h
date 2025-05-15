#ifndef SKETCHWINDOW_H
#define SKETCHWINDOW_H

#include <QMainWindow>

class SketchGLWidget;

class SketchWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit SketchWindow(QWidget* parent = nullptr);

private slots:
    void onFinishShape();
    void onUnion();
    void onIntersection();
    void onSubtraction();

private:
    SketchGLWidget* sketchGLWidget; // Corrected variable name
};

#endif // SKETCHWINDOW_H
