#include "homewidget.h"

HomeWidget::HomeWidget(QWidget *parent) : ModuleWidget(WidgetType::homeWidget,"Home",QColor(255,255,255),parent)
{
    name->setStyleSheet("color:black;");

    int nbMenuWidget = 3;
    menuWidthMin = (this->width() - 50 * (nbMenuWidget+2)) / (nbMenuWidget + 1);
    menuWidthMax = menuWidthMin * 2 + 50;
    int menuHeight = this->height() - 200;
    selectedIndex = 0;
    menuList.push_back(new MenuWidget(WidgetType::galleryWidget,"Gallery",calculatePosY(menuList.size(),selectedIndex), 150,menuWidthMax,menuHeight,":/Resources/gallery.png",QColor(255,123,6),this));
    //menuList.push_back(new MenuWidget(WidgetType::videoplayerWidget,"Videos",calculatePosY(menuList.size(),selectedIndex), 150,menuWidthMin,menuHeight,":/Resources/videos.png",QColor(255,83,96),this));
    menuList.push_back(new MenuWidget(WidgetType::desktopWidget,"Desktop",calculatePosY(menuList.size(),selectedIndex), 150,menuWidthMin,menuHeight,":/Resources/desktop.png",QColor(127,120,114),this));
    menuList.push_back(new MenuWidget(WidgetType::settingsWidget,"Settings",calculatePosY(menuList.size(),selectedIndex), 150,menuWidthMin,menuHeight,":/Resources/settings.png",QColor(120,194,106),this));

    for (int i = 0; i < menuList.size(); i++){
        if (i == selectedIndex)
            menuList[i]->picture->setVisible(true);
        else
            menuList[i]->picture->setVisible(false);
    }
}

HomeWidget::~HomeWidget()
{
    for (int i = 0; i < menuList.size(); i++)
        delete menuList[i];
}

void HomeWidget::switchSelected(bool right)
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

void HomeWidget::switchMenu(QPair<SensorData*,SensorData*> sensorCurrent, QPair<SensorData*,SensorData*> sensorSave, QPair<SensorData*,SensorData*> sensorZero)
{
    if (isVisible()){
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
                    if (menuList[selectedIndex]->getMenuType() == WidgetType::desktopWidget || menuList[selectedIndex]->getMenuType() == WidgetType::galleryWidget || menuList[selectedIndex]->getMenuType() == WidgetType::settingsWidget /*|| menuList[selectedIndex]->getMenuType() == WidgetType::videoplayerWidget*/){
                        emit menuSelected(menuList[selectedIndex]->getMenuType());
                        sensorSave.first->setArm(sensorCurrent.first->getArm());
                        sensorSave.first->setHand(sensorCurrent.first->getHand());
                        selectedIndex = 0;
                        switchSelected(false);
                    }
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
                    if (menuList[selectedIndex]->getMenuType() == WidgetType::desktopWidget || menuList[selectedIndex]->getMenuType() == WidgetType::galleryWidget || menuList[selectedIndex]->getMenuType() == WidgetType::settingsWidget /*|| menuList[selectedIndex]->getMenuType() == WidgetType::videoplayerWidget*/){
                        emit menuSelected(menuList[selectedIndex]->getMenuType());
                        sensorSave.second->setArm(sensorCurrent.second->getArm());
                        sensorSave.second->setHand(sensorCurrent.second->getHand());
                        selectedIndex = 0;
                        switchSelected(false);
                    }
                }
            }
        }
    }
}

int HomeWidget::calculatePosY(int index, int selectedIndex)
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
