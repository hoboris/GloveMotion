#include "mpuvalues.h"

MPUValues::MPUValues()
{
    yaw = -1.0;
    pitch = -1.0;
    roll = -1.0;
}

MPUValues::~MPUValues()
{

}

MPUValues::MPUValues(MPUValues& mpuValues)
{
    yaw = mpuValues.getYaw();
    pitch = mpuValues.getPitch();
    roll = mpuValues.getRoll();
}

MPUValues::MPUValues(float p_yaw, float p_pitch, float p_roll)
{
    /* Normalize the values by converting them in positive-only values so that they can be manipulated more easily for the display
     * Yaw and pitch range goes from [-180,180] to [0,360]
     * Roll range goes from [-90,90] to [0,180]
     */
    yaw = p_yaw + 180;
    pitch = p_pitch + 180;
    roll = p_roll + 90;
}

MPUValues& MPUValues::operator= (MPUValues& mpuValues)
{
    yaw = mpuValues.getYaw();
    pitch = mpuValues.getPitch();
    roll = mpuValues.getRoll();
    return *this;
}

bool MPUValues::operator== (MPUValues& mpuValues)
{
    if (yaw == mpuValues.getYaw() && pitch == mpuValues.getPitch() && roll == mpuValues.getRoll())
        return true;
    else
        return false;
}

bool MPUValues::operator!= (MPUValues& mpuValues)
{
    return !(*this == mpuValues);
}

QString MPUValues::toString()
{
    QString string = "";
    if (isCorrect()) string = "ypr " + QString::number(yaw) + ";" + QString::number(pitch) + ";" + QString::number(roll);
    return string;
}

float MPUValues::getYaw() const
{
    return yaw;
}

void MPUValues::setYaw(float value)
{
    yaw = value;
}
float MPUValues::getPitch() const
{
    return pitch;
}

void MPUValues::setPitch(float value)
{
    pitch = value;
}
float MPUValues::getRoll() const
{
    return roll;
}

void MPUValues::setRoll(float value)
{
    roll = value;
}

void MPUValues::setYawPitchRoll(float p_yaw, float p_pitch, float p_roll)
{
    yaw = p_yaw + 180;
    pitch = p_pitch + 180;
    roll = p_roll + 90;
}

void MPUValues::reset()
{
    yaw = -1.0;
    pitch = -1.0;
    roll = -1.0;
}

bool MPUValues::isCorrect()
{
    if ((yaw == -1 && pitch == -1 && roll == -1) || yaw != yaw || pitch != pitch || roll != roll)
        return false;
    else
        return true;
}
