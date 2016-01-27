#include "settingswidget.h"

SettingsWidget::SettingsWidget(QWidget *parent) : ModuleWidget(WidgetType::settingsWidget,"Settings",QColor(120,194,106),parent)
{
    arduino1Widget = new DataWidget(this);
    arduino1Widget->move(50,200);
    //arduino2Widget = new DataWidget(this);
    //arduino2Widget->move(50,550);

    /*QWidget *line = new QWidget(this);
    line->setFixedHeight(1);
    line->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    line->setStyleSheet(QString("background-color: white;"));
    line->setGeometry(50,500,parent->width()-100,1);*/
}

SettingsWidget::~SettingsWidget()
{
    delete arduino1Widget;
    //delete arduino2Widget;
}

void SettingsWidget::displayData(QPair<SensorData*,SensorData*> sensorCurrent, QPair<SensorData*,SensorData*> sensorSave, QPair<SensorData*,SensorData*> sensorZero)
{
    (void)sensorSave;
    (void)sensorZero;
    if (isVisible()){
        arduino1Widget->drawData(1, sensorCurrent.first);
        //arduino2Widget->drawData(2, sensorCurrent.second);
    }
}
