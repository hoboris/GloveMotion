#include "modulewidget.h"

ModuleWidget::ModuleWidget(WidgetType type, QString text, QColor rgb, QWidget *parent) : QWidget(parent)
{
    widgetType = type;

    name = new QLabel(text, this);
    name->setStyleSheet("color:white;");
    name->setFont(QFont("Segoe UI Light",48));
    name->move(50,30);

    backgroundColor = new QColor(rgb);
    this->setGeometry(0,0,parent->width(),parent->height());
    this->setStyleSheet("background-color:rgb(" + QString::number(backgroundColor->red()) + "," + QString::number(backgroundColor->green()) + "," + QString::number(backgroundColor->blue()) + ");");
}

ModuleWidget::~ModuleWidget()
{
    delete name;
    delete backgroundColor;
}
QLabel *ModuleWidget::getName() const
{
    return name;
}

void ModuleWidget::setName(QLabel *value)
{
    name = value;
}
QColor *ModuleWidget::getBackgroundColor() const
{
    return backgroundColor;
}

void ModuleWidget::setBackgroundColor(QColor *value)
{
    backgroundColor = value;
}
WidgetType ModuleWidget::getWidgetType() const
{
    return widgetType;
}

void ModuleWidget::setWidgetType(const WidgetType &value)
{
    widgetType = value;
}

void ModuleWidget::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void ModuleWidget::collapse(QPair<SensorData*,SensorData*> sensorCurrent, QPair<SensorData*,SensorData*> sensorSave, QPair<SensorData*,SensorData*> sensorZero)
{
    if (isVisible() && this->isActiveWindow()){
        if (sensorCurrent.first->isCorrect() && sensorSave.first->isCorrect()){
            if (sensorCurrent.first->getMiddle()->isLit() && sensorCurrent.first->getIndex()->isLit() &&
                    qAbs(sensorCurrent.first->getHand()->getPitch() - 180) < 45 && qAbs(sensorCurrent.first->getHand()->getRoll() - 90) < 45){
                QPair<int,int> posCurrent = sensorCurrent.first->calculatePos(sensorZero.first,this->width(),this->height());
                QPair<int,int> posSave = sensorSave.first->calculatePos(sensorZero.first,this->width(),this->height());
                if (posSave.second - posCurrent.second < - this->height() / 4){
                    sensorSave.first->setArm(sensorCurrent.first->getArm());
                    sensorSave.first->setHand(sensorCurrent.first->getHand());
                    if (getWidgetType() == WidgetType::homeWidget)
                        QApplication::quit();
                    else
                        emit returnHome(WidgetType::homeWidget);
                }
            }
        }
        if (sensorCurrent.second->isCorrect() && sensorSave.second->isCorrect()){
            if (sensorCurrent.second->getMiddle()->isLit() && sensorCurrent.second->getIndex()->isLit() &&
                    qAbs(sensorCurrent.second->getHand()->getPitch() - 180) < 45 && qAbs(sensorCurrent.second->getHand()->getRoll() - 90) < 45){
                QPair<int,int> posCurrent = sensorCurrent.second->calculatePos(sensorZero.second,this->width(),this->height());
                QPair<int,int> posSave = sensorSave.second->calculatePos(sensorZero.second,this->width(),this->height());
                if (posSave.second - posCurrent.second < - this->height() / 4){
                    sensorSave.second->setArm(sensorCurrent.second->getArm());
                    sensorSave.second->setHand(sensorCurrent.second->getHand());
                    if (getWidgetType() == WidgetType::homeWidget)
                        QApplication::quit();
                    else
                        emit returnHome(WidgetType::homeWidget);
                }
            }
        }
    }
}
