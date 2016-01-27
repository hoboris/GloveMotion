#include "menuwidget.h"
#include <QtWidgets>

MenuWidget::MenuWidget(WidgetType type, QString text, int posX, int posY, int menuWidth, int menuHeight, QString picPath, QColor rgb, QWidget *parent) :QWidget(parent)
{
    this->setGeometry(posX,posY,menuWidth,menuHeight);
    this->setStyleSheet("background-color:rgb(" + QString::number(rgb.red()) + "," + QString::number(rgb.green()) + "," + QString::number(rgb.blue()) + ");");

    menuType = type;
    name = new QLabel(text,this);
    name->setStyleSheet("color:white;");
    name->setFont(QFont("Segoe UI Light",28));
    name->move(20,20);

    QPixmap pixmap(picPath);
    picture = new QLabel(this);
    picture->setPixmap(pixmap);
    picture->setGeometry((this->width() - picture->pixmap()->width()) / 2, (this->height() - picture->pixmap()->height()) / 2, picture->pixmap()->width(), picture->pixmap()->height());
}

void MenuWidget::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

MenuWidget::~MenuWidget()
{
    delete name;
    delete picture;
}
QLabel *MenuWidget::getName() const
{
    return name;
}

void MenuWidget::setName(QLabel *value)
{
    name = value;
}
WidgetType MenuWidget::getMenuType() const
{
    return menuType;
}

void MenuWidget::setMenuType(const WidgetType &value)
{
    menuType = value;
}



