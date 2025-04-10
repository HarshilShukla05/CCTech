#pragma once

#include <QDialog>
#include <QFormLayout>
#include <QLineEdit>
#include <QMap>
#include <QString>

class ShapeInputDialog : public QDialog {
    Q_OBJECT

public:
    explicit ShapeInputDialog(const QString &shapeType, QWidget *parent = nullptr);
    QMap<QString, double> getInputs() const;

private:
    QFormLayout *formLayout;
    QMap<QString, QLineEdit*> inputFields;
};
