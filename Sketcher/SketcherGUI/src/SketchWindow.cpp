#include "SketchWindow.h"
#include "SketchGLWidget.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QInputDialog>

SketchWindow::SketchWindow(QWidget* parent)
    : QWidget(parent) {

    sketchWidget = new SketchGLWidget(this);

    QPushButton* finishShapeBtn = new QPushButton("Finish Shape");
    QPushButton* unionBtn = new QPushButton("Union");
    QPushButton* intersectBtn = new QPushButton("Intersection");
    QPushButton* subtractBtn = new QPushButton("Subtraction");

    connect(finishShapeBtn, &QPushButton::clicked, this, &SketchWindow::onFinishShape);
    connect(unionBtn, &QPushButton::clicked, this, &SketchWindow::onUnion);
    connect(intersectBtn, &QPushButton::clicked, this, &SketchWindow::onIntersection);
    connect(subtractBtn, &QPushButton::clicked, this, &SketchWindow::onSubtraction);

    QHBoxLayout* buttons = new QHBoxLayout;
    buttons->addWidget(finishShapeBtn);
    buttons->addWidget(unionBtn);
    buttons->addWidget(intersectBtn);
    buttons->addWidget(subtractBtn);

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(sketchWidget);
    layout->addLayout(buttons);

    setLayout(layout);
    setWindowTitle("2D Sketch Editor");
    resize(800, 600);
}

void SketchWindow::onFinishShape() { sketchWidget->finishCurrentShape(); }
void SketchWindow::onUnion() { sketchWidget->applyUnion(); }
void SketchWindow::onIntersection() { sketchWidget->applyIntersection(); }
void SketchWindow::onSubtraction() {
    QStringList options = { "A - B", "B - A" };
    bool ok;
    QString choice = QInputDialog::getItem(this, "Subtraction Operation", 
                                           "Choose operation:", options, 0, false, &ok);
    if (ok && !choice.isEmpty()) {
        if (choice == "A - B") {
            sketchWidget->applySubtraction(true);  // A - B
        } else if (choice == "B - A") {
            sketchWidget->applySubtraction(false); // B - A
        }
    }
}
