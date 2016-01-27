#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    this->setWindowState(this->windowState() ^ Qt::WindowFullScreen);

    this->setWindowIcon(QPixmap(":/Resources/icon.png"));

    widgetsList.push_back(new LoadingWidget(this));
    widgetsList.push_back(new SettingsWidget(this));
    widgetsList.push_back(new HomeWidget(this));
    widgetsList.push_back(new GalleryWidget(this));
    //widgetsList.push_back(new VideoplayerWidget(this));
    widgetsList.push_back(new DesktopWidget(this));

    initializeCursors();

    registerMetaTypes();
    startSensorThread();
}

MainWindow::~MainWindow()
{
    for (int i = 0; i < widgetsList.size(); i++)
        delete widgetsList[i];
    delete cursors.first;
    delete cursors.second;
}

void MainWindow::printData(QPair<SensorData*,SensorData*> sensorCurrent, QPair<SensorData*,SensorData*> sensorSave, QPair<SensorData*,SensorData*> sensorZero)
{
    qDebug() << "1) " << sensorCurrent.first->toString() << " 2) " << sensorCurrent.second->toString();
    (void)sensorSave;
    qDebug() << "1) " << sensorZero.first->toString() << " 2) " << sensorZero.second->toString() << "(Zero)";
    (void)sensorZero;
    qDebug() << "1) " << sensorSave.first->toString() << " 2) " << sensorSave.second->toString() << "(Save)";
}

void MainWindow::startSensorThread()
{
    sensorThread = new SensorThread();
    //QObject::connect(sensorThread, SIGNAL(receivedData(QPair<SensorData*,SensorData*>,QPair<SensorData*,SensorData*>,QPair<SensorData*,SensorData*>)), this, SLOT(printData(QPair<SensorData*,SensorData*>,QPair<SensorData*,SensorData*>,QPair<SensorData*,SensorData*>)));
    QObject::connect(sensorThread, SIGNAL(receivedData(QPair<SensorData*,SensorData*>,QPair<SensorData*,SensorData*>,QPair<SensorData*,SensorData*>)), this, SLOT(updateCursors(QPair<SensorData*,SensorData*>,QPair<SensorData*,SensorData*>,QPair<SensorData*,SensorData*>)));
    QObject::connect(sensorThread, SIGNAL(receivedData(QPair<SensorData*,SensorData*>,QPair<SensorData*,SensorData*>,QPair<SensorData*,SensorData*>)), getWidget(WidgetType::settingsWidget), SLOT(displayData(QPair<SensorData*,SensorData*>,QPair<SensorData*,SensorData*>,QPair<SensorData*,SensorData*>)));
    QObject::connect(sensorThread, SIGNAL(receivedData(QPair<SensorData*,SensorData*>,QPair<SensorData*,SensorData*>,QPair<SensorData*,SensorData*>)), getWidget(WidgetType::homeWidget), SLOT(switchMenu(QPair<SensorData*,SensorData*>,QPair<SensorData*,SensorData*>,QPair<SensorData*,SensorData*>)));
    QObject::connect(sensorThread, SIGNAL(receivedData(QPair<SensorData*,SensorData*>,QPair<SensorData*,SensorData*>,QPair<SensorData*,SensorData*>)), getWidget(WidgetType::desktopWidget), SLOT(switchMenu(QPair<SensorData*,SensorData*>,QPair<SensorData*,SensorData*>,QPair<SensorData*,SensorData*>)));
    QObject::connect(sensorThread, SIGNAL(receivedData(QPair<SensorData*,SensorData*>,QPair<SensorData*,SensorData*>,QPair<SensorData*,SensorData*>)), getWidget(WidgetType::galleryWidget), SLOT(displayData(QPair<SensorData*,SensorData*>,QPair<SensorData*,SensorData*>,QPair<SensorData*,SensorData*>)));
    QObject::connect(sensorThread, SIGNAL(connectionChanged(QString)), getWidget(WidgetType::loadingWidget), SLOT(updateSubtitle(QString)));
    QObject::connect(sensorThread, SIGNAL(initializationChanged(WidgetType)), this, SLOT(setActiveWidget(WidgetType)));
    QObject::connect(getWidget(WidgetType::homeWidget), SIGNAL(menuSelected(WidgetType)), this, SLOT(setActiveWidget(WidgetType)));
    QObject::connect(sensorThread, SIGNAL(finished()), sensorThread, SLOT(deleteLater()));
    for (int i = 0; i < widgetsList.size(); i++){
        QObject::connect(sensorThread, SIGNAL(receivedData(QPair<SensorData*,SensorData*>,QPair<SensorData*,SensorData*>,QPair<SensorData*,SensorData*>)), widgetsList[i], SLOT(collapse(QPair<SensorData*,SensorData*>,QPair<SensorData*,SensorData*>,QPair<SensorData*,SensorData*>)));
        QObject::connect(widgetsList[i], SIGNAL(returnHome(WidgetType)), this, SLOT(setActiveWidget(WidgetType)));}
    sensorThread->start();
}

void MainWindow::registerMetaTypes()
{
    qRegisterMetaType<QPair<SensorData*,SensorData*> >("QPair<SensorData*,SensorData*>");
    qRegisterMetaType<WidgetType>("WidgetType");
}

ModuleWidget* MainWindow::getWidget(WidgetType widgetType)
{
    for (int i = 0; i < widgetsList.size(); i++){
        if (widgetsList[i]->getWidgetType() == widgetType)
            return widgetsList[i];
    }
    return 0;
}

void MainWindow::setActiveWidget(WidgetType widgetType)
{
    for (int i = 0; i < widgetsList.size(); i++){
        if (widgetsList[i]->getWidgetType() == widgetType)
            widgetsList[i]->setVisible(true);
        else
            widgetsList[i]->setVisible(false);
    }
}

WidgetType MainWindow::getActiveWidget()
{
    for (int i = 0; i < widgetsList.size(); i++){
        if (widgetsList[i]->isVisible())
            return widgetsList[i]->getWidgetType();
    }
    return WidgetType::noWidget;
}

void MainWindow::initializeCursors()
{
    pixCursors.push_back(new QPixmap(":/Resources/cursor0.png"));
    pixCursors.push_back(new QPixmap(":/Resources/cursor1.png"));
    pixCursors.push_back(new QPixmap(":/Resources/cursor2.png"));
    pixCursors.push_back(new QPixmap(":/Resources/cursor3.png"));
    pixCursors.push_back(new QPixmap(":/Resources/cursor4.png"));
    pixCursors.push_back(new QPixmap(":/Resources/cursor5.png"));
    for (int i = 0; i < pixCursors.size(); i++)
        *pixCursors[i] = pixCursors[i]->scaled(72,72);

    cursors.first = new QLabel(this);
    cursors.second = new QLabel(this);
    cursors.first->setPixmap(*pixCursors[0]);
    cursors.second->setPixmap(*pixCursors[0]);
    cursors.first->setGeometry(this->width()/3,this->height()/2,cursors.first->pixmap()->width(),cursors.first->pixmap()->height());
    cursors.second->setGeometry((this->width()/3)*2,this->height()/2,cursors.second->pixmap()->width(),cursors.second->pixmap()->height());

    cursors.first->raise();
    cursors.second->raise();

    this->setCursor(Qt::BlankCursor);

    QObject::connect(this,SIGNAL(cursorsUpdated(int,int,bool,bool,bool,float)),getWidget(WidgetType::desktopWidget),SLOT(doMouseEvents(int,int,bool,bool,bool,float)));
}

void MainWindow::updateCursors(QPair<SensorData*,SensorData*> sensorCurrent, QPair<SensorData*,SensorData*> sensorSave, QPair<SensorData*,SensorData*> sensorZero)
{
    (void)sensorSave;
    if (sensorCurrent.first->isCorrect() && getActiveWidget() != WidgetType::loadingWidget && getActiveWidget() != WidgetType::noWidget){
        cursors.first->setVisible(true);
        /* Hand opened position */
        if (!sensorCurrent.first->getThumb()->isSelected() && !sensorCurrent.first->getIndex()->isSelected() & !sensorCurrent.first->getMiddle()->isSelected()){
            QPixmap p(pixCursors[0]->transformed(QTransform().rotate(sensorCurrent.first->getArm()->getPitch() - 180)));
            cursors.first->setPixmap(p);
        }
        /* Selected hand opened position */
        else if (sensorCurrent.first->getThumb()->isSelected() && !sensorCurrent.first->getIndex()->isSelected() & !sensorCurrent.first->getMiddle()->isSelected()){
            QPixmap p(pixCursors[1]->transformed(QTransform().rotate(sensorCurrent.first->getArm()->getPitch() - 180)));
            cursors.first->setPixmap(p);
        }
        /* Pointing position */
        else if (!sensorCurrent.first->getThumb()->isSelected() && !sensorCurrent.first->getIndex()->isSelected() & sensorCurrent.first->getMiddle()->isSelected()){
            QPixmap p(pixCursors[4]->transformed(QTransform().rotate(sensorCurrent.first->getArm()->getPitch() - 180)));
            cursors.first->setPixmap(p);
        }
        /* Selected pointing position */
        else if (sensorCurrent.first->getThumb()->isSelected() && !sensorCurrent.first->getIndex()->isSelected() & sensorCurrent.first->getMiddle()->isSelected()){
            QPixmap p(pixCursors[2]->transformed(QTransform().rotate(sensorCurrent.first->getArm()->getPitch() - 180)));
            cursors.first->setPixmap(p);
        }
        /* Hand closed zero position */
        else if (sensorCurrent.first->getThumb()->isSelected() && sensorCurrent.first->getIndex()->isSelected() & sensorCurrent.first->getMiddle()->isSelected()){
            QPixmap p(pixCursors[5]->transformed(QTransform().rotate(sensorCurrent.first->getArm()->getPitch() - 180)));
            cursors.first->setPixmap(p);
        }
        /* Calculate the cursor's abciss from the arm's MPU6050 yaw value
         * Calculate the cursor's ordinate from the arm's MPU6050 roll value*/
        QPair<int,int> pos = sensorCurrent.first->calculatePos(sensorZero.first,this->width(),this->height());
        cursors.first->setGeometry(pos.first,pos.second,cursors.first->pixmap()->width(),cursors.first->pixmap()->height());
        emit cursorsUpdated(pos.first,pos.second,sensorCurrent.first->getThumb()->isSelected(),sensorCurrent.first->getIndex()->isSelected(),sensorCurrent.first->getMiddle()->isSelected(),sensorCurrent.first->getHand()->getPitch());
    }
    /* Hide the cursor if the Arduino is not connected or we are in the loading screen */
    else
        cursors.first->setVisible(false);
    /* Do the same thing for the second Arduino */
    if (sensorCurrent.second->isCorrect() && getActiveWidget() != WidgetType::loadingWidget && getActiveWidget() != WidgetType::noWidget){
        cursors.second->setVisible(true);
        if (!sensorCurrent.second->getThumb()->isSelected() && !sensorCurrent.second->getIndex()->isSelected() & !sensorCurrent.second->getMiddle()->isSelected()){
            QPixmap p(pixCursors[0]->transformed(QTransform().rotate(sensorCurrent.second->getArm()->getPitch() - 180)));
            cursors.second->setPixmap(p);
        }
        else if (sensorCurrent.second->getThumb()->isSelected() && !sensorCurrent.second->getIndex()->isSelected() & !sensorCurrent.second->getMiddle()->isSelected()){
            QPixmap p(pixCursors[1]->transformed(QTransform().rotate(sensorCurrent.second->getArm()->getPitch() - 180)));
            cursors.second->setPixmap(p);
        }
        else if (!sensorCurrent.second->getThumb()->isSelected() && !sensorCurrent.second->getIndex()->isSelected() & sensorCurrent.second->getMiddle()->isSelected()){
            QPixmap p(pixCursors[4]->transformed(QTransform().rotate(sensorCurrent.second->getArm()->getPitch() - 180)));
            cursors.second->setPixmap(p);
        }
        else if (sensorCurrent.second->getThumb()->isSelected() && !sensorCurrent.second->getIndex()->isSelected() & sensorCurrent.second->getMiddle()->isSelected()){
            QPixmap p(pixCursors[2]->transformed(QTransform().rotate(sensorCurrent.second->getArm()->getPitch() - 180)));
            cursors.second->setPixmap(p);
        }
        else if (sensorCurrent.second->getThumb()->isSelected() && sensorCurrent.second->getIndex()->isSelected() & sensorCurrent.second->getMiddle()->isSelected()){
            QPixmap p(pixCursors[5]->transformed(QTransform().rotate(sensorCurrent.second->getArm()->getPitch() - 180)));
            cursors.second->setPixmap(p);
        }
        QPair<int,int> pos = sensorCurrent.second->calculatePos(sensorZero.first,this->width(),this->height());
        cursors.second->setGeometry(pos.first,pos.second,cursors.second->pixmap()->width(),cursors.second->pixmap()->height());
    }
    else
        cursors.second->setVisible(false);
}
