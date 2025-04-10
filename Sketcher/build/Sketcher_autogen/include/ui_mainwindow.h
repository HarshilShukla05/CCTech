/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QComboBox *shapeComboBox;
    QWidget *openGLContainer;
    QHBoxLayout *bottomButtonLayout;
    QPushButton *loadFileButton;
    QPushButton *saveFileButton;
    QPushButton *transformButton;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName("verticalLayout");
        shapeComboBox = new QComboBox(centralwidget);
        shapeComboBox->addItem(QString());
        shapeComboBox->addItem(QString());
        shapeComboBox->addItem(QString());
        shapeComboBox->addItem(QString());
        shapeComboBox->addItem(QString());
        shapeComboBox->addItem(QString());
        shapeComboBox->addItem(QString());
        shapeComboBox->setObjectName("shapeComboBox");

        verticalLayout->addWidget(shapeComboBox);

        openGLContainer = new QWidget(centralwidget);
        openGLContainer->setObjectName("openGLContainer");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(openGLContainer->sizePolicy().hasHeightForWidth());
        openGLContainer->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(openGLContainer);

        bottomButtonLayout = new QHBoxLayout();
        bottomButtonLayout->setObjectName("bottomButtonLayout");
        loadFileButton = new QPushButton(centralwidget);
        loadFileButton->setObjectName("loadFileButton");

        bottomButtonLayout->addWidget(loadFileButton);

        saveFileButton = new QPushButton(centralwidget);
        saveFileButton->setObjectName("saveFileButton");

        bottomButtonLayout->addWidget(saveFileButton);

        transformButton = new QPushButton(centralwidget);
        transformButton->setObjectName("transformButton");

        bottomButtonLayout->addWidget(transformButton);


        verticalLayout->addLayout(bottomButtonLayout);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 21));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Shape Plotter", nullptr));
        shapeComboBox->setItemText(0, QCoreApplication::translate("MainWindow", "Cuboid", nullptr));
        shapeComboBox->setItemText(1, QCoreApplication::translate("MainWindow", "Sphere", nullptr));
        shapeComboBox->setItemText(2, QCoreApplication::translate("MainWindow", "Cylinder", nullptr));
        shapeComboBox->setItemText(3, QCoreApplication::translate("MainWindow", "Polygon", nullptr));
        shapeComboBox->setItemText(4, QCoreApplication::translate("MainWindow", "Bezier", nullptr));
        shapeComboBox->setItemText(5, QCoreApplication::translate("MainWindow", "Polyline", nullptr));
        shapeComboBox->setItemText(6, QCoreApplication::translate("MainWindow", "3DLine", nullptr));

        loadFileButton->setText(QCoreApplication::translate("MainWindow", "Load File", nullptr));
        saveFileButton->setText(QCoreApplication::translate("MainWindow", "Save File", nullptr));
        transformButton->setText(QCoreApplication::translate("MainWindow", "Transform", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
