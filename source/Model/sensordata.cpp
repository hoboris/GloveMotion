#include "sensordata.h"

SensorData::SensorData()
{
    arm = new MPUValues();
    hand = new MPUValues();
    thumb = new LDRValue();
    index = new LDRValue();
    middle = new LDRValue();
}

SensorData::~SensorData()
{
    delete arm;
    delete hand;
    delete thumb;
    delete index;
    delete middle;
}
MPUValues *SensorData::getArm() const
{
    return arm;
}

void SensorData::setArm(MPUValues *value)
{
    arm->setYaw(value->getYaw());
    arm->setPitch(value->getPitch());
    arm->setRoll(value->getRoll());
}
MPUValues *SensorData::getHand() const
{
    return hand;
}

void SensorData::setHand(MPUValues *value)
{
    hand->setYaw(value->getYaw());
    hand->setPitch(value->getPitch());
    hand->setRoll(value->getRoll());
}

LDRValue *SensorData::getThumb() const
{
    return thumb;
}

void SensorData::setThumb(LDRValue *value)
{
    thumb->setValue(value->getValue());
}
LDRValue *SensorData::getIndex() const
{
    return index;
}

void SensorData::setIndex(LDRValue *value)
{
    index->setValue(value->getValue());
}
LDRValue *SensorData::getMiddle() const
{
    return middle;
}

void SensorData::setMiddle(LDRValue *value)
{
    middle->setValue(value->getValue());
}

void SensorData::setAll(QStringList stringData)
{
        if (stringData.size() >= 3)
            arm->setYawPitchRoll(stringData[0].toFloat(),stringData[1].toFloat(),stringData[2].toFloat());
        else
            arm->reset();
        if (stringData.size() >= 6)
            hand->setYawPitchRoll(stringData[3].toFloat(),stringData[4].toFloat(),stringData[5].toFloat());
        else
            hand->reset();
        if (stringData.size() == 9){
            thumb->setValue(stringData[6].toInt());
            index->setValue(stringData[7].toInt());
            middle->setValue(stringData[8].toInt());
        }
        else{
            thumb->reset();
            index->reset();
            middle->reset();
        }
}

bool SensorData::isCorrect()
{
    if (arm->isCorrect() || hand->isCorrect() || thumb->isCorrect() || index->isCorrect() || middle->isCorrect())
        return true;
    else
        return false;
}

QString SensorData::toString()
{
    QString string;
    string = "1" + arm->toString() + " ; 2" + hand->toString() + " ; ldr " + thumb->toString() + ";" + index->toString() + ";" + middle->toString();
    return string;
}

void SensorData::reset()
{
    arm->reset();
    hand->reset();
    thumb->reset();
    index->reset();
    middle->reset();
}

QPair<int,int> SensorData::calculatePos(SensorData* sensorZero, int windowWidth, int windowHeight)
{
    QPair<int,int> pos;
    int zero, min, max, val;
    if (isCorrect() && sensorZero->isCorrect()){
        // Yaw
        zero = static_cast<int>(sensorZero->getArm()->getYaw());
        val = static_cast<int>(arm->getYaw());
        min = static_cast<int>(((zero - 45) % 360 + 360) % 360);
        max = static_cast<int>(((zero + 45) % 360 + 360) % 360);
        if (zero > max){
            min = ((min - max) % 360 + 360) % 360;
            zero = ((zero - max) % 360 + 360) % 360;
            val = ((val - max) % 360 + 360) % 360;
            max = 360;
        }
        else if (zero < min){
            zero = ((zero - min) % 360 + 360) % 360;
            max = ((max - min) % 360 + 360) % 360;
            val = ((val - min) % 360 + 360) % 360;
            min = 0;
        }
        if (val > min && val < max){
            pos.first = ((static_cast<float>(val - min) / static_cast<float>(max - min)) * windowWidth);
        }
        else if (val <= min){
            pos.first = 0;
        }
        else if (val >= max){
            pos.first = windowWidth;
        }

        // Roll
        zero = static_cast<int>(sensorZero->getArm()->getRoll());
        val = static_cast<int>(arm->getRoll());
        min = static_cast<int>(((zero - 45) % 180 + 180) % 180);
        max = static_cast<int>(((zero + 45) % 180 + 180) % 180);
        if (zero > max){
            min = ((min - max) % 180 + 180) % 180;
            zero = ((zero - max) % 180 + 180) % 180;
            val = ((val - max) % 180 + 180) % 180;
            max = 180;
        }
        else if (zero < min){
            zero = ((zero - min) % 180 + 180) % 180;
            max = ((max - min) % 180 + 180) % 180;
            val = ((val - min) % 180 + 180) % 180;
            min = 0;
        }
        if (val > min && val < max){
            pos.second = ((static_cast<float>(val - min) / static_cast<float>(max - min)) * windowHeight);
        }
        else if (val <= min){
            pos.second = 0;
        }
        else if (val >= max){
            pos.second = windowHeight;
        }

        return pos;
    }
    else{
        pos.first = -1;
        pos.second = -1;
        return pos;
    }
}
