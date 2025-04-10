/********************************************************************************
** Form generated from reading UI file 'ShapeInputDialog.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SHAPEINPUTDIALOG_H
#define UI_SHAPEINPUTDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ShapeInputDialog
{
public:
    QVBoxLayout *verticalLayout;
    QComboBox *shapeComboBox;
    QStackedWidget *stackedWidget;
    QWidget *cuboidPage;
    QFormLayout *formLayoutCuboid;
    QLabel *labelLength;
    QLineEdit *lengthEdit;
    QLabel *labelWidth;
    QLineEdit *widthEdit;
    QLabel *labelHeight;
    QLineEdit *heightEdit;
    QWidget *spherePage;
    QFormLayout *formLayoutSphere;
    QLabel *labelRadius;
    QLineEdit *radiusEdit;
    QWidget *cylinderPage;
    QFormLayout *formLayoutCylinder;
    QLabel *labelRadiusCyl;
    QLineEdit *radiusCylEdit;
    QLabel *labelHeightCyl;
    QLineEdit *heightCylEdit;
    QHBoxLayout *buttonLayout;
    QPushButton *okButton;
    QPushButton *cancelButton;

    void setupUi(QDialog *ShapeInputDialog)
    {
        if (ShapeInputDialog->objectName().isEmpty())
            ShapeInputDialog->setObjectName("ShapeInputDialog");
        ShapeInputDialog->resize(300, 200);
        verticalLayout = new QVBoxLayout(ShapeInputDialog);
        verticalLayout->setObjectName("verticalLayout");
        shapeComboBox = new QComboBox(ShapeInputDialog);
        shapeComboBox->addItem(QString());
        shapeComboBox->addItem(QString());
        shapeComboBox->addItem(QString());
        shapeComboBox->setObjectName("shapeComboBox");

        verticalLayout->addWidget(shapeComboBox);

        stackedWidget = new QStackedWidget(ShapeInputDialog);
        stackedWidget->setObjectName("stackedWidget");
        cuboidPage = new QWidget();
        cuboidPage->setObjectName("cuboidPage");
        formLayoutCuboid = new QFormLayout(cuboidPage);
        formLayoutCuboid->setObjectName("formLayoutCuboid");
        labelLength = new QLabel(cuboidPage);
        labelLength->setObjectName("labelLength");

        formLayoutCuboid->setWidget(0, QFormLayout::ItemRole::LabelRole, labelLength);

        lengthEdit = new QLineEdit(cuboidPage);
        lengthEdit->setObjectName("lengthEdit");

        formLayoutCuboid->setWidget(0, QFormLayout::ItemRole::FieldRole, lengthEdit);

        labelWidth = new QLabel(cuboidPage);
        labelWidth->setObjectName("labelWidth");

        formLayoutCuboid->setWidget(1, QFormLayout::ItemRole::LabelRole, labelWidth);

        widthEdit = new QLineEdit(cuboidPage);
        widthEdit->setObjectName("widthEdit");

        formLayoutCuboid->setWidget(1, QFormLayout::ItemRole::FieldRole, widthEdit);

        labelHeight = new QLabel(cuboidPage);
        labelHeight->setObjectName("labelHeight");

        formLayoutCuboid->setWidget(2, QFormLayout::ItemRole::LabelRole, labelHeight);

        heightEdit = new QLineEdit(cuboidPage);
        heightEdit->setObjectName("heightEdit");

        formLayoutCuboid->setWidget(2, QFormLayout::ItemRole::FieldRole, heightEdit);

        stackedWidget->addWidget(cuboidPage);
        spherePage = new QWidget();
        spherePage->setObjectName("spherePage");
        formLayoutSphere = new QFormLayout(spherePage);
        formLayoutSphere->setObjectName("formLayoutSphere");
        labelRadius = new QLabel(spherePage);
        labelRadius->setObjectName("labelRadius");

        formLayoutSphere->setWidget(0, QFormLayout::ItemRole::LabelRole, labelRadius);

        radiusEdit = new QLineEdit(spherePage);
        radiusEdit->setObjectName("radiusEdit");

        formLayoutSphere->setWidget(0, QFormLayout::ItemRole::FieldRole, radiusEdit);

        stackedWidget->addWidget(spherePage);
        cylinderPage = new QWidget();
        cylinderPage->setObjectName("cylinderPage");
        formLayoutCylinder = new QFormLayout(cylinderPage);
        formLayoutCylinder->setObjectName("formLayoutCylinder");
        labelRadiusCyl = new QLabel(cylinderPage);
        labelRadiusCyl->setObjectName("labelRadiusCyl");

        formLayoutCylinder->setWidget(0, QFormLayout::ItemRole::LabelRole, labelRadiusCyl);

        radiusCylEdit = new QLineEdit(cylinderPage);
        radiusCylEdit->setObjectName("radiusCylEdit");

        formLayoutCylinder->setWidget(0, QFormLayout::ItemRole::FieldRole, radiusCylEdit);

        labelHeightCyl = new QLabel(cylinderPage);
        labelHeightCyl->setObjectName("labelHeightCyl");

        formLayoutCylinder->setWidget(1, QFormLayout::ItemRole::LabelRole, labelHeightCyl);

        heightCylEdit = new QLineEdit(cylinderPage);
        heightCylEdit->setObjectName("heightCylEdit");

        formLayoutCylinder->setWidget(1, QFormLayout::ItemRole::FieldRole, heightCylEdit);

        stackedWidget->addWidget(cylinderPage);

        verticalLayout->addWidget(stackedWidget);

        buttonLayout = new QHBoxLayout();
        buttonLayout->setObjectName("buttonLayout");
        okButton = new QPushButton(ShapeInputDialog);
        okButton->setObjectName("okButton");

        buttonLayout->addWidget(okButton);

        cancelButton = new QPushButton(ShapeInputDialog);
        cancelButton->setObjectName("cancelButton");

        buttonLayout->addWidget(cancelButton);


        verticalLayout->addLayout(buttonLayout);


        retranslateUi(ShapeInputDialog);

        QMetaObject::connectSlotsByName(ShapeInputDialog);
    } // setupUi

    void retranslateUi(QDialog *ShapeInputDialog)
    {
        ShapeInputDialog->setWindowTitle(QCoreApplication::translate("ShapeInputDialog", "Enter Shape Parameters", nullptr));
        shapeComboBox->setItemText(0, QCoreApplication::translate("ShapeInputDialog", "Cuboid", nullptr));
        shapeComboBox->setItemText(1, QCoreApplication::translate("ShapeInputDialog", "Sphere", nullptr));
        shapeComboBox->setItemText(2, QCoreApplication::translate("ShapeInputDialog", "Cylinder", nullptr));

        labelLength->setText(QCoreApplication::translate("ShapeInputDialog", "Length:", nullptr));
        labelWidth->setText(QCoreApplication::translate("ShapeInputDialog", "Width:", nullptr));
        labelHeight->setText(QCoreApplication::translate("ShapeInputDialog", "Height:", nullptr));
        labelRadius->setText(QCoreApplication::translate("ShapeInputDialog", "Radius:", nullptr));
        labelRadiusCyl->setText(QCoreApplication::translate("ShapeInputDialog", "Radius:", nullptr));
        labelHeightCyl->setText(QCoreApplication::translate("ShapeInputDialog", "Height:", nullptr));
        okButton->setText(QCoreApplication::translate("ShapeInputDialog", "OK", nullptr));
        cancelButton->setText(QCoreApplication::translate("ShapeInputDialog", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ShapeInputDialog: public Ui_ShapeInputDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SHAPEINPUTDIALOG_H
