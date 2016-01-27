#ifndef HOMEWIDGET_H
#define HOMEWIDGET_H

#include <QWidget>
#include <QPair>
#include <QList>
#include "menuwidget.h"
#include "../modulewidget.h"
#include "../../Model/sensordata.h"

class HomeWidget : public ModuleWidget
{
    Q_OBJECT
public:
    explicit HomeWidget(QWidget *parent = 0);
    ~HomeWidget();

private:
    QList<MenuWidget*> menuList;
    int selectedIndex;
    int menuWidthMin, menuWidthMax;

    int calculatePosY(int index, int selectedIndex);
    void switchSelected(bool right);

signals:
    void menuSelected(WidgetType);

public slots:
    void switchMenu(QPair<SensorData*,SensorData*> sensorCurrent, QPair<SensorData*,SensorData*> sensorSave, QPair<SensorData*,SensorData*> sensorZero);
};

#endif // HOMEWIDGET_H
