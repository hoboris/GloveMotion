#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H

#include <QtWidgets>
#include <QPair>

#include "../../Model/sensordata.h"
#include "../modulewidget.h"
#include "datawidget.h"

class SettingsWidget : public ModuleWidget
{
    Q_OBJECT
public:
    explicit SettingsWidget(QWidget *parent = 0);
    ~SettingsWidget();

private:
    DataWidget *arduino1Widget/*, *arduino2Widget*/;

signals:

public slots:
    void displayData(QPair<SensorData*,SensorData*> sensorCurrent, QPair<SensorData*,SensorData*> sensorSave, QPair<SensorData*,SensorData*> sensorZero);
};

#endif // SETTINGSWIDGET_H
