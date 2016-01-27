#ifndef MPUVALUES_H
#define MPUVALUES_H

#include <QString>

class MPUValues
{
public:
    MPUValues();
    ~MPUValues();
    MPUValues(MPUValues& mpuValues);
    MPUValues(float p_yaw, float p_pitch, float p_roll);

    MPUValues& operator= (MPUValues& mpuValues);
    bool operator== (MPUValues& mpuValues);
    bool operator!= (MPUValues& mpuValues);
    QString toString();

    float getYaw() const;
    void setYaw(float value);
    float getPitch() const;
    void setPitch(float value);
    float getRoll() const;
    void setRoll(float value);
    void setYawPitchRoll(float p_yaw, float p_pitch, float p_roll);
    void reset();

    bool isCorrect();

private:
    float yaw;
    float pitch;
    float roll;
};

#endif // MPUVALUES_H
