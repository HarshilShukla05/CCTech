#include "ShapeInputDialog.h"
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QDoubleValidator>
#include <QDebug>

ShapeInputDialog::ShapeInputDialog(const QString &shapeType, QWidget *parent)
    : QDialog(parent) {
    setWindowTitle("Enter " + shapeType + " dimensions");

    QFormLayout *formLayout = new QFormLayout;
    QStringList parameters;

    if (shapeType == "Cuboid") {
        parameters = {"Length", "Width", "Height"};
    } else if (shapeType == "Cylinder") {
        parameters = {"Radius", "Height"};
    } else if (shapeType == "Sphere") {
        parameters = {"Radius"};
    }

    for (const QString &param : parameters) {
        QLineEdit *lineEdit = new QLineEdit(this);
        lineEdit->setValidator(new QDoubleValidator(0, 1000, 3, lineEdit));
        formLayout->addRow(param + ":", lineEdit);
        inputFields[param] = lineEdit;
    }

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(buttonBox);

    setLayout(mainLayout);
}

QMap<QString, double> ShapeInputDialog::getInputs() const {
    QMap<QString, double> values;
    for (auto it = inputFields.begin(); it != inputFields.end(); ++it) {
        values[it.key()] = it.value()->text().toDouble();
    }
    return values;
}
