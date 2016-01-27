#ifndef DESKTOPWIDGET_H
#define DESKTOPWIDGET_H

#include <QtWidgets>
#include <Windows.h>
#include <QProcess>
#include <QStringList>
#include <QList>
#include <QElapsedTimer>
#include "menuwidget.h"
#include "../modulewidget.h"
#include "../../Model/sensordata.h"

class DesktopWidget : public ModuleWidget
{
    Q_OBJECT
public:
    explicit DesktopWidget(QWidget *parent = 0);
    ~DesktopWidget();

private:
    bool thumbSave, indexSave, middleSave;
    QList<MenuWidget*> menuList;
    int selectedIndex;
    int menuWidthMin, menuWidthMax;
    QList<QProcess*> processList;

    int calculatePosY(int index, int selectedIndex);
    void switchSelected(bool right);
    void launchApp(QString appName);

signals:

public slots:
    void doMouseEvents(int posX, int posY, bool thumb, bool index, bool middle, float pitch);
    void switchMenu(QPair<SensorData*,SensorData*> sensorCurrent, QPair<SensorData*,SensorData*> sensorSave, QPair<SensorData*,SensorData*> sensorZero);
};

#endif // DESKTOPWIDGET_H
