#ifndef SENSORTHREAD_H
#define SENSORTHREAD_H

#include <QString>
#include <QStringList>
#include <QList>
#include <QPair>
#include <QDebug>
#include <QElapsedTimer>

#include <QThread>

#include <QSerialPort>
#include <QtSerialPort/QtSerialPort>
#include <QSerialPortInfo>

#include "../Model/sensordata.h"
#include "../View/modulewidget.h"

class SensorThread : public QThread
{
    Q_OBJECT

public:
    SensorThread();
    ~SensorThread();

    QList<QPair<SensorData *, SensorData *> > getSensorBuffer() const;

private:
    void run();
    bool checkInitialized();
    void removeUnusualData();
    void checkGestures();
    bool isCorrect(QStringList splittedData);
    QPair<SensorData*,SensorData*> sensorCurrent, sensorSave, sensorZero;

signals:
    /* Send current sensor data, save data and zero data */
    void receivedData(QPair<SensorData*,SensorData*>,QPair<SensorData*,SensorData*>,QPair<SensorData*,SensorData*>);
    void deselected(QPair<SensorData*,SensorData*>,QPair<SensorData*,SensorData*>,QPair<SensorData*,SensorData*>);
    void connectionChanged(QString);
    void initializationChanged(WidgetType);
};

#endif // SENSORTHREAD_H
