#ifndef DATAWIDGET_H
#define DATAWIDGET_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QGraphicsView>
#include "../../Model/sensordata.h"

namespace Ui {
class DataWidget;
}

class DataWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DataWidget(QWidget *parent = 0);
    ~DataWidget();
    void drawData(int num = 0, SensorData* sensorData = 0);
    Ui::DataWidget *ui;
    QGraphicsScene scene_1, scene_2, scene_3;

private:
    void paintEvent(QPaintEvent *);
    void changeLabelsColor();
    QGraphicsEllipseItem *circle1, *circle2, *circle3;
    QGraphicsLineItem *line1, *line2, *line3, *line4, *line5, *line6;
};

#endif // DATAWIDGET_H
