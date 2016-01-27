#ifndef SENSORDATA_H
#define SENSORDATA_H

#include <QStringList>
#include <QDebug>
#include <QMetaType>

#include "ldrvalue.h"
#include "mpuvalues.h"

class SensorData
{
public:
    SensorData();
    ~SensorData();

    MPUValues *getArm() const;
    void setArm(MPUValues *value);

    MPUValues *getHand() const;
    void setHand(MPUValues *value);

    void setAll(QStringList stringData);

    LDRValue *getThumb() const;
    void setThumb(LDRValue *value);

    LDRValue *getIndex() const;
    void setIndex(LDRValue *value);

    LDRValue *getMiddle() const;
    void setMiddle(LDRValue *value);

    void reset();

    bool isCorrect();
    QString toString();

    QPair<int,int> calculatePos(SensorData* sensorZero, int windowWidth, int windowHeight);

private:
    MPUValues *arm, *hand;
    LDRValue *thumb, *index, *middle;
};

Q_DECLARE_METATYPE(SensorData)

#endif // SENSORDATA_H
