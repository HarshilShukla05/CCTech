#include "TransformDialogue.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QVBoxLayout>

TransformDialog::TransformDialog(QWidget *parent)
    : QDialog(parent) {
    setWindowTitle("Transform Shape");
    
    // QVBoxLayout *mainLayout = new QVBoxLayout(this);

    translateCheck = new QCheckBox("Translate");
    rotateCheck = new QCheckBox("Rotate");
    scaleCheck = new QCheckBox("Scale");

    dxSpin = new QDoubleSpinBox(); dxSpin->setRange(-1000, 1000);
    dySpin = new QDoubleSpinBox(); dySpin->setRange(-1000, 1000);
    dzSpin = new QDoubleSpinBox(); dzSpin->setRange(-1000, 1000);

    QFormLayout *translateLayout = new QFormLayout;
    translateLayout->addRow("dx:", dxSpin);
    translateLayout->addRow("dy:", dySpin);
    translateLayout->addRow("dz:", dzSpin);

    angleSpin = new QDoubleSpinBox(); angleSpin->setRange(-360, 360);
    axisBox = new QComboBox();
    axisBox->addItems({"x", "y", "z"});

    QFormLayout *rotateLayout = new QFormLayout;
    rotateLayout->addRow("Angle:", angleSpin);
    rotateLayout->addRow("Axis:", axisBox);

    sxSpin = new QDoubleSpinBox(); sxSpin->setRange(0.1, 100);
    sySpin = new QDoubleSpinBox(); sySpin->setRange(0.1, 100);
    szSpin = new QDoubleSpinBox(); szSpin->setRange(0.1, 100);

    QFormLayout *scaleLayout = new QFormLayout;
    scaleLayout->addRow("sx:", sxSpin);
    scaleLayout->addRow("sy:", sySpin);
    scaleLayout->addRow("sz:", szSpin);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(translateCheck);
    layout->addLayout(translateLayout);
    layout->addWidget(rotateCheck);
    layout->addLayout(rotateLayout);
    layout->addWidget(scaleCheck);
    layout->addLayout(scaleLayout);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    layout->addWidget(buttonBox);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    setLayout(layout);
}

TransformDialog::~TransformDialog() {}

bool TransformDialog::isTranslateChecked() const { return translateCheck->isChecked(); }
bool TransformDialog::isRotateChecked() const { return rotateCheck->isChecked(); }
bool TransformDialog::isScaleChecked() const { return scaleCheck->isChecked(); }

double TransformDialog::dx() const { return dxSpin->value(); }
double TransformDialog::dy() const { return dySpin->value(); }
double TransformDialog::dz() const { return dzSpin->value(); }

double TransformDialog::angle() const { return angleSpin->value(); }
char TransformDialog::axis() const { return axisBox->currentText().toStdString()[0]; }

double TransformDialog::sx() const { return sxSpin->value(); }
double TransformDialog::sy() const { return sySpin->value(); }
double TransformDialog::sz() const { return szSpin->value(); }

QMap<QString, double> TransformDialog::getValues() const {
    QMap<QString, double> values;
    if (isTranslateChecked()) {
        values["dx"] = dx();
        values["dy"] = dy();
        values["dz"] = dz();
    }
    if (isRotateChecked()) {
        values["angle"] = angle();
        values["axis"] = axis();
    }
    if (isScaleChecked()) {
        values["sx"] = sx();
        values["sy"] = sy();
        values["sz"] = sz();
    }
    return values;
}

QStringList TransformDialog::getSelectedTransforms() const {
    QStringList selected;
    if (isTranslateChecked()) {
        selected.append("Translate");
    }
    if (isRotateChecked()) {
        selected.append("Rotate");
    }
    if (isScaleChecked()) {
        selected.append("Scale");
    }
    return selected;
}
