#pragma once

#include <QMainWindow>
#include <Bezier.h>
#include "SketchWindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class GLWidget;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    
private slots:
    void onShapeSelected(const QString &shape);
    void on_transformButton_clicked();
    void applyTransformations(const std::vector<std::vector<double>> &verts);
    void on_loadFileButton_clicked();
    void on_saveFileButton_clicked();
    void on_openSketchWindowButton_clicked();
    void on_stlIntersection_triggered();

    
    void setBezierShape(Bezier* bezier) { bezierShape = bezier; update(); }

    
private:
    Ui::MainWindow *ui;
    GLWidget *glWidget;
    Bezier* bezierShape = nullptr;
    SketchWindow* sketchWindow;
};