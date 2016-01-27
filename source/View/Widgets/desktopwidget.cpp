#include "desktopwidget.h"

DesktopWidget::DesktopWidget(QWidget *parent) : ModuleWidget(WidgetType::desktopWidget,"Desktop",QColor(127,120,114),parent)
{
    thumbSave = false;
    indexSave = false;
    middleSave = false;

    int nbMenuWidget = 5;
    menuWidthMin = (this->width() - 50 * (nbMenuWidget+2)) / (nbMenuWidget + 1);
    menuWidthMax = menuWidthMin * 2 + 50;
    int menuHeight = this->height() - 200;
    selectedIndex = 0;
    menuList.push_back(new MenuWidget(WidgetType::noWidget,"G. Maps",calculatePosY(menuList.size(),selectedIndex), 150,menuWidthMax,menuHeight,":/Resources/maps.png",QColor(12,202,191),this));
    menuList.push_back(new MenuWidget(WidgetType::noWidget,"G. Earth",calculatePosY(menuList.size(),selectedIndex), 150,menuWidthMin,menuHeight,":/Resources/earth.png",QColor(226,50,125),this));
    menuList.push_back(new MenuWidget(WidgetType::noWidget,"Paint",calculatePosY(menuList.size(),selectedIndex), 150,menuWidthMin,menuHeight,":/Resources/paint.png",QColor(244,188,65),this));
    menuList.push_back(new MenuWidget(WidgetType::noWidget,"2048",calculatePosY(menuList.size(),selectedIndex), 150,menuWidthMin,menuHeight,":/Resources/2048.png",QColor(150,110,170),this));
    menuList.push_back(new MenuWidget(WidgetType::noWidget,"Archery",calculatePosY(menuList.size(),selectedIndex), 150,menuWidthMin,menuHeight,":/Resources/archery.png",QColor(240,198,220),this));

    for (int i = 0; i < menuList.size(); i++){
        if (i == selectedIndex)
            menuList[i]->picture->setVisible(true);
        else
            menuList[i]->picture->setVisible(false);
    }
}

DesktopWidget::~DesktopWidget()
{
    if (thumbSave)
        mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTUP,0,0,0,0);
    if (middleSave)
        mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_RIGHTUP,0,0,0,0);
    if (indexSave)
        mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MIDDLEUP,0,0,0,0);
    /*for (int i = 0; i < processList.size(); i++){
        if (processList[i]->program() != "C:/Program Files (x86)/Google/Chrome/Application/chrome.exe")
            delete processList[i];
    }*/
}

void DesktopWidget::switchSelected(bool right)
{
    /* True for right, false for left */
    if (right == true && selectedIndex < menuList.size() - 1)
        selectedIndex++;
    else if (right == false && selectedIndex > 0)
        selectedIndex--;
    for (int i = 0; i < menuList.size(); i++){
        if (i == selectedIndex){
            menuList[i]->setGeometry(calculatePosY(i,selectedIndex), 150,menuWidthMax,this->height() - 200);
            menuList[i]->picture->setVisible(true);
            menuList[i]->picture->setGeometry((menuList[i]->width() - menuList[i]->picture->pixmap()->width()) / 2, (menuList[i]->height() - menuList[i]->picture->pixmap()->height()) / 2, menuList[i]->picture->pixmap()->width(), menuList[i]->picture->pixmap()->height());
        }
        else{
            menuList[i]->setGeometry(calculatePosY(i,selectedIndex), 150,menuWidthMin,this->height() - 200);
            menuList[i]->picture->setVisible(false);
            menuList[i]->picture->setGeometry((menuList[i]->width() - menuList[i]->picture->pixmap()->width()) / 2, (menuList[i]->height() - menuList[i]->picture->pixmap()->height()) / 2, menuList[i]->picture->pixmap()->width(), menuList[i]->picture->pixmap()->height());
        }
    }
}

void DesktopWidget::switchMenu(QPair<SensorData*,SensorData*> sensorCurrent, QPair<SensorData*,SensorData*> sensorSave, QPair<SensorData*,SensorData*> sensorZero)
{
    if (isVisible() && this->isActiveWindow()){
        this->setCursor(Qt::BlankCursor);
        if (sensorCurrent.first->isCorrect() && sensorSave.first->isCorrect()){
            if ((qAbs(sensorCurrent.first->getHand()->getPitch() - 90) < 45 || qAbs(sensorCurrent.first->getHand()->getPitch() - 270) < 45) &&
                    qAbs(sensorCurrent.first->getHand()->getRoll() - 90) < 45){
                QPair<int,int> posCurrent = sensorCurrent.first->calculatePos(sensorZero.first,this->width(),this->height());
                QPair<int,int> posSave = sensorSave.first->calculatePos(sensorZero.first,this->width(),this->height());
                if (posSave.first - posCurrent.first < - this->width() / 8){
                    sensorSave.first->setArm(sensorCurrent.first->getArm());
                    sensorSave.first->setHand(sensorCurrent.first->getHand());
                    switchSelected(false);
                }
                else if (posSave.first - posCurrent.first > this->width() / 8){
                    sensorSave.first->setArm(sensorCurrent.first->getArm());
                    sensorSave.first->setHand(sensorCurrent.first->getHand());
                    switchSelected(true);
                }
            }
            else if (qAbs(sensorCurrent.first->getHand()->getRoll()) < 45 && qAbs(sensorCurrent.first->getHand()->getPitch() - 180) < 45){
                QPair<int,int> posCurrent = sensorCurrent.first->calculatePos(sensorZero.first,this->width(),this->height());
                QPair<int,int> posSave = sensorSave.first->calculatePos(sensorZero.first,this->width(),this->height());
                if (posSave.second - posCurrent.second < - this->height() / 10){
                    launchApp(menuList[selectedIndex]->getName()->text());
                    sensorSave.first->setArm(sensorCurrent.first->getArm());
                    sensorSave.first->setHand(sensorCurrent.first->getHand());
                }
            }
        }
        if (sensorCurrent.second->isCorrect() && sensorSave.second->isCorrect()){
            if ((qAbs(sensorCurrent.second->getHand()->getPitch() - 90) < 45 || qAbs(sensorCurrent.second->getHand()->getPitch() - 270) < 45) &&
                    qAbs(sensorCurrent.second->getHand()->getRoll() - 90) < 45){
                QPair<int,int> posCurrent = sensorCurrent.second->calculatePos(sensorZero.second,this->width(),this->height());
                QPair<int,int> posSave = sensorSave.second->calculatePos(sensorZero.second,this->width(),this->height());
                if (posSave.first - posCurrent.first < - this->width() / 8){
                    sensorSave.second->setArm(sensorCurrent.second->getArm());
                    sensorSave.second->setHand(sensorCurrent.second->getHand());
                    switchSelected(false);
                }
                else if (posSave.first - posCurrent.first > this->width() / 8){
                    sensorSave.second->setArm(sensorCurrent.second->getArm());
                    sensorSave.second->setHand(sensorCurrent.second->getHand());
                    switchSelected(true);
                }
            }
            else if (qAbs(sensorCurrent.second->getHand()->getRoll()) < 45 && qAbs(sensorCurrent.second->getHand()->getPitch() - 180) < 45){
                QPair<int,int> posCurrent = sensorCurrent.second->calculatePos(sensorZero.second,this->width(),this->height());
                QPair<int,int> posSave = sensorSave.second->calculatePos(sensorZero.second,this->width(),this->height());
                if (posSave.second - posCurrent.second < - this->height() / 10){
                    launchApp(menuList[selectedIndex]->getName()->text());
                    sensorSave.second->setArm(sensorCurrent.second->getArm());
                    sensorSave.second->setHand(sensorCurrent.second->getHand());
                }
            }
        }
    }
    else if (isVisible() && !this->isActiveWindow()){
        if (sensorCurrent.first->isCorrect() && sensorSave.first->isCorrect()){
            if (qAbs(sensorCurrent.first->getHand()->getRoll()) < 45 && qAbs(sensorCurrent.first->getHand()->getPitch() - 180) < 45){
                QPair<int,int> posCurrent = sensorCurrent.first->calculatePos(sensorZero.first,this->width(),this->height());
                QPair<int,int> posSave = sensorSave.first->calculatePos(sensorZero.first,this->width(),this->height());
                if (posSave.second - posCurrent.second < - this->height() / 10){
                    this->activateWindow();
                    sensorSave.first->setArm(sensorCurrent.first->getArm());
                    sensorSave.first->setHand(sensorCurrent.first->getHand());
                    if (thumbSave){
                        thumbSave = false;
                        mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTUP,0,0,0,0);
                    }
                    if (middleSave){
                        middleSave = false;
                        mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_RIGHTUP,0,0,0,0);
                    }
                    if (indexSave){
                        indexSave = false;
                        mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MIDDLEUP,0,0,0,0);
                    }
                }
            }
        }
    }
}

int DesktopWidget::calculatePosY(int index, int selectedIndex)
{
    if (index == 0)
        return 50;
    else{
        if (selectedIndex < index)
            return 50 * (index + 1) + menuWidthMax + menuWidthMin * (index - 1);
        else
            return 50 * (index + 1) + menuWidthMin + menuWidthMin * (index - 1);
    }
}

void DesktopWidget::doMouseEvents(int posX, int posY, bool thumb, bool index, bool middle, float pitch)
{
    if (isVisible() && !this->isActiveWindow()){
        QApplication::desktop()->cursor().setPos(posX,posY);
        if (thumb && !thumbSave){
            thumbSave = thumb;
            mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN,posX,posY,0,0);
        }
        else if (!thumb && thumbSave){
            thumbSave = thumb;
            mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTUP,posX,posY,0,0);
        }
        if (middle && !middleSave){
            middleSave = middle;
            mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_RIGHTDOWN,posX,posY,0,0);
        }
        else if (!middle && middleSave){
            middleSave = middle;
            mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_RIGHTUP,posX,posY,0,0);
        }
        if (index && !indexSave){
            indexSave = index;
            mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MIDDLEDOWN,posX,posY,0,0);
        }
        else if (!index && indexSave){
            indexSave = index;
            mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MIDDLEUP,posX,posY,0,0);
        }
        if (pitch < 180 - 25 || pitch > 180 + 25)
            mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_WHEEL,posX,posY,pitch - 180,0);
    }
}

void DesktopWidget::launchApp(QString appName)
{
    QString program;
    QStringList arguments;
    if (appName == "G. Maps"){
        program = "C:/Program Files (x86)/Google/Chrome/Application/chrome.exe";
        arguments << "www.google.com/maps/@40.7219517,-73.9667117,10z";
    }
    else if (appName == "G. Earth")
        program = "C:/Program Files (x86)/Google/Google Earth/client/googleearth.exe";
    else if (appName == "Paint")
        program = "C:/Windows/system32/mspaint.exe";
    else if (appName == "2048"){
        program = "C:/Program Files (x86)/Google/Chrome/Application/chrome.exe";
        arguments << "www.medalgames.com/files/swf/7.swf";
    }
    else if (appName == "Archery"){
        program = "C:/Program Files (x86)/Google/Chrome/Application/chrome.exe";
        arguments << "data6.gamepuma.com/games/10/max-arrow-archery.swf";
    }
    if (!appName.isEmpty()){
        processList.push_back(new QProcess(this));
        processList.last()->start(program, arguments);
    }
}
