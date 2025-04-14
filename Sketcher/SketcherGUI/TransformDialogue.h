#pragma once

#include <QDialog>
#include <QCheckBox>
#include <QFormLayout>
#include <QDoubleSpinBox>
#include <QDialogButtonBox>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QCombobox>
#include <QWidget>
#include <QMap>
#include <QStringList>


class TransformDialog : public QDialog {
    Q_OBJECT

public:
    explicit TransformDialog(QWidget *parent = nullptr);
    ~TransformDialog();

    bool isTranslateChecked() const;
    bool isRotateChecked() const;
    bool isScaleChecked() const;

    double dx() const;
    double dy() const;
    double dz() const;

    double angle() const;
    char axis() const;

    double sx() const;
    double sy() const;
    double sz() const;

    QMap<QString, double> getValues() const;
    QStringList getSelectedTransforms() const;

private:
    QCheckBox *translateCheck;
    QCheckBox *rotateCheck;
    QCheckBox *scaleCheck;

    QDoubleSpinBox *dxSpin, *dySpin, *dzSpin;
    QDoubleSpinBox *angleSpin;
    QDoubleSpinBox *sxSpin, *sySpin, *szSpin;
    QComboBox *axisBox;
};
