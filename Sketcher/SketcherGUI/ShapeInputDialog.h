#pragma once

#include <QDialog>
#include <QFormLayout>
#include <QLineEdit>
#include <QMap>
#include <QString>
#include <QSpinBox>
#include <QVector>

class ShapeInputDialog : public QDialog {
    Q_OBJECT

public:
    explicit ShapeInputDialog(const QString &shapeType, QWidget *parent = nullptr);
    QMap<QString, double> getInputs() const;
    QVector<QVector<double>> getBezierControlPoints() const;
    QVector<QVector<double>> getPolylinePoints() const;
    int getInterpolationPoints() const;

private:
    QFormLayout *formLayout;
    QMap<QString, QLineEdit*> inputFields;

    //For Bezier control points
    QSpinBox *interpolationPointSpinBox;
    QSpinBox *controlPointCountSpinBox = nullptr;
    QVector<QVector<QLineEdit*>> bezierControlFields;
    QSpinBox *numberOfSidesSpinBox = nullptr;
};
