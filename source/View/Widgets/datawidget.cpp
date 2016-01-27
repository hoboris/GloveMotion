#include "datawidget.h"
#include "ui_datawidget.h"

DataWidget::DataWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DataWidget)
{
    ui->setupUi(this);

    ui->graphicsView->setScene(&scene_1);
    ui->graphicsView_2->setScene(&scene_2);
    ui->graphicsView_3->setScene(&scene_3);

    QPen pen;
    pen.setColor(Qt::white);
    pen.setWidth(2);

    scene_1.addRect(QRect(0,0,185,20),pen, QBrush(Qt::NoBrush));
    scene_1.addRect(QRect(0,40,185,20),pen, QBrush(Qt::NoBrush));
    scene_1.addRect(QRect(0,80,185,20),pen, QBrush(Qt::NoBrush));

    scene_2.addRect(QRect(0,0,185,20),pen, QBrush(Qt::NoBrush));
    scene_2.addRect(QRect(0,40,185,20),pen, QBrush(Qt::NoBrush));
    scene_2.addRect(QRect(0,80,185,20),pen, QBrush(Qt::NoBrush));

    line1 = scene_1.addLine(QLineF(0.5*185,0,0.5*185,20),pen);
    line2 = scene_1.addLine(QLineF(0.5*185,40,0.5*185,60),pen);
    line3 = scene_1.addLine(QLineF(0.5*185,80,0.5*185,100),pen);

    line4 = scene_2.addLine(QLineF(0.5*185,0,0.5*185,20),pen);
    line5 = scene_2.addLine(QLineF(0.5*185,40,0.5*185,60),pen);
    line6 = scene_2.addLine(QLineF(0.5*185,80,0.5*185,100),pen);

    circle1 = scene_3.addEllipse(QRect(0,0,30,30),pen);
    circle2 = scene_3.addEllipse(QRect(0,35,30,30),pen);
    circle3 = scene_3.addEllipse(QRect(0,70,30,30),pen);

    changeLabelsColor();
    drawData();
}

DataWidget::~DataWidget()
{
    delete ui;
    delete line1;
    delete line2;
    delete line3;
    delete line4;
    delete line5;
    delete line6;
    delete circle1;
    delete circle2;
    delete circle3;
}

void DataWidget::changeLabelsColor()
{
    ui->label->setStyleSheet("color:white;");
    ui->label_2->setStyleSheet("color:white;");
    ui->label_3->setStyleSheet("color:white;");
    ui->label_4->setStyleSheet("color:white;");
    ui->label_5->setStyleSheet("color:white;");
    ui->label_6->setStyleSheet("color:white;");
    ui->label_7->setStyleSheet("color:white;");
    ui->label_8->setStyleSheet("color:white;");
    ui->label_9->setStyleSheet("color:white;");
    ui->label_10->setStyleSheet("color:white;");
    ui->label_11->setStyleSheet("color:white;");
    ui->label_12->setStyleSheet("color:white;");
    ui->label_13->setStyleSheet("color:white;");
    ui->label_14->setStyleSheet("color:white;");
}

void DataWidget::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void DataWidget::drawData(int num, SensorData* sensorData)
{
    if (sensorData != 0){
        if (sensorData->isCorrect() && (num == 1 || num == 2)){
            ui->label->setText("Arduino " + QString::number(num));
            if (sensorData->getArm()->isCorrect()){
                ui->label_12->setText(QString::number(static_cast<int>(sensorData->getArm()->getYaw() - 180)) + "\n\n"
                                      + QString::number(static_cast<int>(sensorData->getArm()->getPitch() - 180)) + "\n\n"
                                      + QString::number(static_cast<int>(sensorData->getArm()->getRoll() - 90)) );
                line1->setLine(QLineF((sensorData->getArm()->getYaw()/360.0)*185,0,(sensorData->getArm()->getYaw()/360.0)*185,20));
                line2->setLine(QLineF((sensorData->getArm()->getPitch()/360.0)*185,40,(sensorData->getArm()->getPitch()/360.0)*185,60));
                line3->setLine(QLineF((sensorData->getArm()->getRoll()/180.0)*185,80,(sensorData->getArm()->getRoll()/180.0)*185,100));
            }
            if (sensorData->getHand()->isCorrect()){
                ui->label_13->setText(QString::number(static_cast<int>(sensorData->getHand()->getYaw() - 180)) + "\n\n"
                                      + QString::number(static_cast<int>(sensorData->getHand()->getPitch() - 180)) + "\n\n"
                                      + QString::number(static_cast<int>(sensorData->getHand()->getRoll() - 90)) );
                line4->setLine(QLineF((sensorData->getHand()->getYaw()/360.0)*185,0,(sensorData->getHand()->getYaw()/360.0)*185,20));
                line5->setLine(QLineF((sensorData->getHand()->getPitch()/360.0)*185,40,(sensorData->getHand()->getPitch()/360.0)*185,60));
                line6->setLine(QLineF((sensorData->getHand()->getRoll()/180.0)*185,80,(sensorData->getHand()->getRoll()/180.0)*185,100));
            }
            if (sensorData->getThumb()->isCorrect() && sensorData->getIndex()->isCorrect() && sensorData->getMiddle()->isCorrect()){
                ui->label_14->setText(QString::number(static_cast<int>(sensorData->getThumb()->getValue() / 10)) + "\n\n"
                                      + QString::number(static_cast<int>(sensorData->getIndex()->getValue() / 10)) + "\n\n"
                                      + QString::number(static_cast<int>(sensorData->getMiddle()->getValue() / 10)) );
            }
            if (!sensorData->getThumb()->isSelected())
                circle1->setBrush(QBrush(Qt::white));
            else
                circle1->setBrush(QBrush(Qt::NoBrush));
            if (!sensorData->getIndex()->isSelected())
                circle2->setBrush(QBrush(Qt::white));
            else
                circle2->setBrush(QBrush(Qt::NoBrush));
            if (!sensorData->getMiddle()->isSelected())
                circle3->setBrush(QBrush(Qt::white));
            else
                circle3->setBrush(QBrush(Qt::NoBrush));
        }
        else{
            ui->label->setText("Arduino " + QString::number(num) + " (not connected)");
            ui->label_12->setText("");
            ui->label_13->setText("");
            ui->label_14->setText("");
            line1->setLine(QLineF(0.5*185,0,0.5*185,20));
            line2->setLine(QLineF(0.5*185,40,0.5*185,60));
            line3->setLine(QLineF(0.5*185,80,0.5*185,100));
            line4->setLine(QLineF(0.5*185,0,0.5*185,20));
            line5->setLine(QLineF(0.5*185,40,0.5*185,60));
            line6->setLine(QLineF(0.5*185,80,0.5*185,100));
            circle1->setBrush(QBrush(Qt::NoBrush));
            circle2->setBrush(QBrush(Qt::NoBrush));
            circle3->setBrush(QBrush(Qt::NoBrush));
        }
    }
    else{
        ui->label->setText("Arduino " + QString::number(num) + " (not connected)");
        ui->label_12->setText("");
        ui->label_13->setText("");
        ui->label_14->setText("");
        line1->setLine(QLineF(0.5*185,0,0.5*185,20));
        line2->setLine(QLineF(0.5*185,40,0.5*185,60));
        line3->setLine(QLineF(0.5*185,80,0.5*185,100));
        line4->setLine(QLineF(0.5*185,0,0.5*185,20));
        line5->setLine(QLineF(0.5*185,40,0.5*185,60));
        line6->setLine(QLineF(0.5*185,80,0.5*185,100));
        circle1->setBrush(QBrush(Qt::NoBrush));
        circle2->setBrush(QBrush(Qt::NoBrush));
        circle3->setBrush(QBrush(Qt::NoBrush));
    }
}
