#ifndef SKETCHWINDOW_H
#define SKETCHWINDOW_H

#include <QWidget>

class SketchGLWidget;

class SketchWindow : public QWidget {
    Q_OBJECT

public:
    explicit SketchWindow(QWidget* parent = nullptr);

private slots:
    void onFinishShape();
    void onUnion();
    void onIntersection();
    void onSubtraction();

private:
    SketchGLWidget* sketchWidget;
};

#endif // SKETCHWINDOW_H
