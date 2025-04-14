#include "ShapeInputDialog.h"
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QDoubleValidator>
#include <QSpinBox>
#include <QLabel>
#include <QDebug>

ShapeInputDialog::ShapeInputDialog(const QString &shapeType, QWidget *parent)
    : QDialog(parent) {
    setWindowTitle("Enter " + shapeType + " dimensions");

    QFormLayout *formLayout = new QFormLayout;
    QStringList parameters;

    if (shapeType == "Cuboid") {
        parameters = {"Length", "Width", "Height"};
    } else if (shapeType == "Cylinder") {
        parameters = {"Radius", "Height", "Resolution"};
    } else if (shapeType == "Sphere") {
        parameters = {"Radius", "Segments"};
    } else if (shapeType == "Bezier") {
        // Control point count input
        QLabel *label = new QLabel("Number of Control Points:");
        controlPointCountSpinBox = new QSpinBox(this);
        controlPointCountSpinBox->setMinimum(2);
        controlPointCountSpinBox->setMaximum(20);
        formLayout->addRow(label, controlPointCountSpinBox);

        // Dynamically generate inputs on value change
        connect(controlPointCountSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, [=](int count) {
            // Remove previous fields
            for (auto row : bezierControlFields) {
                for (auto field : row) {
                    formLayout->removeWidget(field);
                    delete field;
                }
            }
            bezierControlFields.clear();

            // Add new fields
            for (int i = 0; i < count; ++i) {
                QVector<QLineEdit*> row;
                for (const QString &axis : {"X", "Y", "Z"}) {
                    QLineEdit *coordInput = new QLineEdit(this);
                    coordInput->setValidator(new QDoubleValidator(-1000, 1000, 3, coordInput));
                    formLayout->addRow(QString("P%1 %2:").arg(i).arg(axis), coordInput);
                    row.append(coordInput);
                }
                bezierControlFields.append(row);
            }
        });
    }
    else if (shapeType == "Polygon") {
        parameters = {"Number of Sides", "Radius"};
    }
    else if (shapeType == "3DLine") {
        parameters = {"X1", "Y1", "Z1", "X2", "Y2", "Z2"};
    }
    else if (shapeType == "Polyline") {
        parameters = {"Number of Points"};
        numberOfSidesSpinBox = new QSpinBox(this);
        numberOfSidesSpinBox->setMinimum(2);
        numberOfSidesSpinBox->setMaximum(20);
        formLayout->addRow("Number of Points:", numberOfSidesSpinBox);

        connect(numberOfSidesSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, [=](int count) {
            // Remove previous fields
            for (auto row : bezierControlFields) {
                for (auto field : row) {
                    formLayout->removeWidget(field);
                    delete field;
                }
            }
            bezierControlFields.clear();

            // Add new fields
            for (int i = 0; i < count; ++i) {
                QVector<QLineEdit*> row;
                for (const QString &axis : {"X", "Y", "Z"}) {
                    QLineEdit *coordInput = new QLineEdit(this);
                    coordInput->setValidator(new QDoubleValidator(-1000, 1000, 3, coordInput));
                    formLayout->addRow(QString("P%1 %2:").arg(i).arg(axis), coordInput);
                    row.append(coordInput);
                }
                bezierControlFields.append(row);
            }
        });
    }
    else if (shapeType == "Triangle") {
        parameters = {"X1", "Y1", "Z1", "X2", "Y2", "Z2", "X3", "Y3", "Z3"};
    }

    // For non-Bezier shapes
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

QVector<QVector<double>> ShapeInputDialog::getBezierControlPoints() const {
    QVector<QVector<double>> controlPoints;
    for (const auto &row : bezierControlFields) {
        QVector<double> point;
        for (const auto &field : row) {
            point.append(field->text().toDouble());
        }
        controlPoints.append(point);
    }
    return controlPoints;
}

QVector<QVector<double>> ShapeInputDialog::getPolylinePoints() const {
    QVector<QVector<double>> points;
    for (const auto &row : bezierControlFields) {
        QVector<double> point;
        for (const auto &field : row) {
            point.append(field->text().toDouble());
        }
        points.append(point);
    }
    return points;
}
