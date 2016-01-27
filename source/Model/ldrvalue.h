#ifndef LDRVALUE_H
#define LDRVALUE_H

#include <QString>
#include <QDebug>
#include <QFile>

#define PATH "sensitivity.txt"
#define DEFAULT_SENSITIVITY 50

class LDRValue
{
public:
    LDRValue();
    ~LDRValue();
    LDRValue(LDRValue& ldrValue);
    LDRValue(int p_value);
    LDRValue& operator= (LDRValue& ldrValue);
    bool operator== (LDRValue& ldrValue);
    bool operator!= (LDRValue& ldrValue);
    QString toString();
    int readSensitivity();

    int getValue() const;
    void setValue(int value);
    void reset();
    bool isLit();
    bool isSelected();
    bool isCorrect();

    int getSensitivity() const;
    void setSensitivity(int value);

private:
    int value;
    int sensitivity;
};

#endif // LDRVALUE_H
