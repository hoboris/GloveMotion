#include "ldrvalue.h"

LDRValue::LDRValue()
{
    value = -1;
    sensitivity = readSensitivity();
}

LDRValue::~LDRValue()
{

}

LDRValue::LDRValue(LDRValue& ldrValue)
{
    value = ldrValue.getValue();
    sensitivity = ldrValue.getSensitivity();
}

LDRValue::LDRValue(int p_value)
{
    value = p_value;
    sensitivity = readSensitivity();
}

LDRValue& LDRValue::operator= (LDRValue& ldrValue)
{
    value = ldrValue.getValue();
    sensitivity = ldrValue.getSensitivity();
    return *this;
}

bool LDRValue::operator== (LDRValue& ldrValue)
{
    if (value == ldrValue.getValue())
        return true;
    else return false;
}

bool LDRValue::operator!= (LDRValue& ldrValue)
{
    return !(*this == ldrValue);
}

QString LDRValue::toString()
{
    QString string = "";
    if (isCorrect()) string = QString::number(value);
    return string;
}

int LDRValue::getValue() const
{
    return value;
}

void LDRValue::setValue(int value)
{
    this->value = value;
}

void LDRValue::reset()
{
    value = -1;
}

bool LDRValue::isLit()
{
    if (value == -1)
        return false;
    else if (value > sensitivity)
        return true;
    else
        return false;
}

bool LDRValue::isSelected()
{
    return !isLit();
}

bool LDRValue::isCorrect()
{
    if (value == -1)
        return false;
    else
        return true;
}
int LDRValue::getSensitivity() const
{
    return sensitivity;
}

void LDRValue::setSensitivity(int value)
{
    sensitivity = value;
}


int LDRValue::readSensitivity()
{
    QFile file(PATH);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        if (file.isOpen()) file.close();
        return DEFAULT_SENSITIVITY;
    }
    if (!file.atEnd()) {
        QByteArray ba = file.readLine();
        QString rawData = ba.data();
        qDebug() << rawData.toInt();
        if (file.isOpen()) file.close();
        return rawData.toInt();
    }
    if (file.isOpen()) file.close();
    return DEFAULT_SENSITIVITY;
}
