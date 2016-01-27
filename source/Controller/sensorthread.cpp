#include "sensorthread.h"

SensorThread::SensorThread()
{
    sensorCurrent.first = new SensorData();
    sensorCurrent.second = new SensorData();
    sensorZero.first = new SensorData();
    sensorZero.second = new SensorData();
    sensorSave.first = new SensorData();
    sensorSave.second = new SensorData();
}

SensorThread::~SensorThread()
{
    delete sensorCurrent.first;
    delete sensorCurrent.second;
    delete sensorSave.first;
    delete sensorSave.second;
    delete sensorZero.first;
    delete sensorZero.second;
}

void SensorThread::run()
{
    //emit initializationChanged(WidgetType::homeWidget);
    //return;
    WidgetType widgetToLoad = WidgetType::homeWidget;
    QList<QSerialPortInfo> infoList;
    QSerialPortInfo info1, info2;
    QSerialPort serial1, serial2;
    int nbArduinos;
    int currentNbArduinos;
    QString rawData1,rawData2;
    QStringList splittedData1, splittedData2;
    QByteArray ba1, ba2;
    QSerialPortInfo info;
    QElapsedTimer timer;
    timer.start();
    while(1){
        infoList =  QSerialPortInfo::availablePorts();
        nbArduinos = 0;
        emit initializationChanged(WidgetType::loadingWidget);

        /* Check how many Arduinos are connected
         * Initialize the serial information for the connected Arduinos */
        foreach (info, infoList){
            if (info.description() == "Arduino Uno" || info.description() == "USB-SERIAL CH340"){
                if (nbArduinos == 0){
                    info1 = info;
                    nbArduinos++;
                }
                else if (nbArduinos == 1){
                    info2 = info;
                    nbArduinos++;
                }
                else
                    nbArduinos++;
            }
        }

        if (nbArduinos == 0){
            emit connectionChanged("No Arduinos are connected");
            continue;
        }
        else if (nbArduinos > 2){
            emit connectionChanged("More than two Arduinos are connected");
            continue;
        }
        serial1.setPort(info1);
        serial1.setBaudRate(QSerialPort::Baud115200);
        if (nbArduinos == 2){
            serial2.setPort(info2);
            serial2.setBaudRate(QSerialPort::Baud115200);
        }

        if ((nbArduinos == 1 && serial1.open(QIODevice::ReadWrite)) || (nbArduinos == 2 && serial1.open(QIODevice::ReadWrite) && serial2.open(QIODevice::ReadWrite))){
            rawData1 = "";
            rawData2 = "";
            splittedData1.clear();
            splittedData2.clear();
            timer.restart();
            while(serial1.waitForReadyRead(10) == false){
                /* No data has been received, we need to unplug replug the Arduinos to reinitialize them */
                if (timer.elapsed() > 3000){
                    emit connectionChanged("Connection problem, replug the Arduino");
                    break;
                }
            }
            if (timer.elapsed() > 3000){
                if (serial1.isOpen()) serial1.close();
                if (serial2.isOpen()) serial2.close();
                continue;
            }
            else timer.restart();
            /* At the beginning the Arduino buffer is not cleared, so we get rid of the first corrupted data flow */
            serial1.readAll();
            if (nbArduinos == 2){
                while(serial2.waitForReadyRead(10) == false){
                    if (timer.elapsed() > 1000){
                        emit connectionChanged("Connection problem, replug the Arduino");
                        break;
                    }
                }
                if (timer.elapsed() > 1000){
                    if (serial1.isOpen()) serial1.close();
                    if (serial2.isOpen()) serial2.close();
                    continue;
                }
                else timer.restart();
                serial2.readAll();
            }

            while (timer.elapsed() < 1000) {}
            timer.restart();

            emit initializationChanged(widgetToLoad);

            while(1){
                /* Check if a second Arduino has been plugged meanwhile */
                currentNbArduinos = 0;
                infoList =  QSerialPortInfo::availablePorts();
                foreach (info, infoList){
                    if (info.description() == "Arduino Uno" || info.description() == "USB-SERIAL CH340"){
                        currentNbArduinos++;
                    }
                }
                if (currentNbArduinos != nbArduinos){
                    if (serial1.isOpen()) serial1.close();
                    if (serial2.isOpen()) serial2.close();
                    break;
                }

                while(serial1.waitForReadyRead(10) == false){
                    /* The Arduinos has been disconnected while in use */
                    if (info1.isValid() == false)
                        break;
                }
                if (info1.isValid() == false){
                    if (serial1.isOpen()) serial1.close();
                    if (serial2.isOpen()) serial2.close();
                    break;
                }
                ba1 = serial1.readAll();
                rawData1 += ba1.data();
                ba1.clear();

                if (rawData1.endsWith("\n")) {
                    rawData1.remove('\n');
                    splittedData1 = rawData1.split(";");
                    if (isCorrect(splittedData1))
                        sensorCurrent.first->setAll(splittedData1);
                    else
                        sensorCurrent.first->reset();
                    rawData1.clear();
                    splittedData1.clear();
                }

                if (nbArduinos == 2){
                    while(serial2.waitForReadyRead(10) == false){
                        if (info2.isValid() == false)
                            break;
                    }
                    if (info2.isValid() == false){
                        if (serial1.isOpen()) serial1.close();
                        if (serial2.isOpen()) serial2.close();
                        break;
                    }
                    ba2 = serial2.readAll();
                    rawData2 += ba2.data();
                    ba2.clear();
                    if (rawData2.endsWith("\n")) {
                        rawData2.remove('\n');
                        splittedData2 = rawData2.split(";");
                        if (isCorrect(splittedData2))
                            sensorCurrent.second->setAll(splittedData2);
                        else
                            sensorCurrent.second->reset();
                        rawData2.clear();
                        splittedData2.clear();
                    }
                }
                else{
                    sensorCurrent.second->reset();
                }

                if (sensorCurrent.first->isCorrect() && (nbArduinos == 1 || sensorCurrent.second->isCorrect())){
                    checkGestures();
                }
            }
            if (serial1.isOpen()) serial1.close();
            if (serial2.isOpen()) serial2.close();
        }
        else{
            //emit connectionChanged("Loading ...");
            if (serial1.isOpen()) serial1.close();
            if (serial2.isOpen()) serial2.close();
            continue;
        }
    }
}

void SensorThread::checkGestures()
{
    /* Zero */
    if (sensorZero.first->isCorrect() == false){
        sensorZero.first->setArm(sensorCurrent.first->getArm());
        sensorZero.first->setHand(sensorCurrent.first->getHand());
    }
    if (sensorCurrent.second->isCorrect() && sensorZero.second->isCorrect() == false){
        sensorZero.second->setArm(sensorCurrent.second->getArm());
        sensorZero.second->setHand(sensorCurrent.second->getHand());
    }
    if (sensorCurrent.first->getThumb()->isSelected() && sensorCurrent.first->getIndex()->isSelected() && sensorCurrent.first->getMiddle()->isSelected()){
        sensorZero.first->setArm(sensorCurrent.first->getArm());
        sensorZero.first->setHand(sensorCurrent.first->getHand());
        sensorSave.first->reset();
    }
    if (sensorCurrent.second->isCorrect() && sensorCurrent.second->getThumb()->isSelected() && sensorCurrent.second->getIndex()->isSelected() && sensorCurrent.second->getMiddle()->isSelected()){
        sensorZero.second->setArm(sensorCurrent.second->getArm());
        sensorZero.second->setHand(sensorCurrent.second->getHand());
        sensorSave.second->reset();
    }
    /* Check save */
    if (sensorCurrent.first->getThumb()->isSelected() && !sensorCurrent.first->getIndex()->isSelected() && sensorSave.first->isCorrect() == false){
        sensorSave.first->setArm(sensorCurrent.first->getArm());
        sensorSave.first->setHand(sensorCurrent.first->getHand());
    }
    if (sensorCurrent.second->isCorrect() && sensorCurrent.second->getThumb()->isSelected() && !sensorCurrent.second->getIndex()->isSelected() && sensorSave.second->isCorrect() == false){
        sensorSave.second->setArm(sensorCurrent.second->getArm());
        sensorSave.second->setHand(sensorCurrent.second->getHand());
    }
    /* Check unsave */
    if (!sensorCurrent.first->getThumb()->isSelected() && sensorSave.first->isCorrect()){
        sensorSave.first->reset();
    }
    if (sensorCurrent.second->isCorrect() && !sensorCurrent.second->getThumb()->isSelected() && sensorSave.second->isCorrect()){
        sensorSave.second->reset();
    }
    emit receivedData(sensorCurrent,sensorSave,sensorZero);
}

bool SensorThread::isCorrect(QStringList splittedData)
{
    if (splittedData.size() == 3 || splittedData.size() == 6 || splittedData.size() == 9){
        for (int i = 0; i < splittedData.size(); i++){
            if (i >= 6){
                int value = splittedData[i].toInt();
                if (value != value || value < 0)
                    return false;
            }
            else {
                float value = splittedData[i].toFloat();
                if (value != value)
                    return false;
                else if (i != 2 && i != 5 && (value + 180 < 0))
                    return false;
                else if ((i == 2 || i == 5) && (value + 90 < 0))
                    return false;
            }
        }
        return true;
    }
    else
        return false;
}
