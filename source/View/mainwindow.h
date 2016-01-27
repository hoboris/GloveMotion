#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <QPair>
#include <QString>
#include <QList>
#include <QLabel>
#include <QTimer>

#include "../Model/sensordata.h"
#include "../Controller/sensorthread.h"
#include "../View/modulewidget.h"
#include "../View/Widgets/loadingwidget.h"
#include "../View/Widgets/settingswidget.h"
#include "../View/Widgets/homewidget.h"
#include "../View/Widgets/gallerywidget.h"
#include "../View/Widgets/videoplayerwidget.h"
#include "../View/Widgets/desktopwidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    QList<ModuleWidget*> widgetsList;
    QPair<QLabel*,QLabel*> cursors;
    ~MainWindow();
    ModuleWidget* getWidget(WidgetType widgetType);
    WidgetType getActiveWidget();

private:
    SensorThread* sensorThread;
    QList<QPixmap*> pixCursors;
    void initializeCursors();
    void startSensorThread();
    void registerMetaTypes();

signals:
    void cursorsUpdated(int,int,bool,bool,bool,float);

public slots:
    void printData(QPair<SensorData*,SensorData*> sensorCurrent, QPair<SensorData*,SensorData*> sensorSave, QPair<SensorData *, SensorData *> sensorZero);
    void updateCursors(QPair<SensorData*,SensorData*> sensorCurrent, QPair<SensorData*,SensorData*> sensorSave, QPair<SensorData*,SensorData*> sensorZero);
    void setActiveWidget(WidgetType widgetType);
};

#endif // MAINWINDOW_H
