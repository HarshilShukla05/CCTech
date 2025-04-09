#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "GLWidget.h"

#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    setCentralWidget(ui->centralwidget);
    glWidget = new GLWidget(this);
    ui->openGLContainer->setLayout(new QVBoxLayout);
    ui->openGLContainer->layout()->addWidget(glWidget);
}

MainWindow::~MainWindow() {
    delete ui;
}
