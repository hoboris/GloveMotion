#ifndef MENUWIDGET_H
#define MENUWIDGET_H

#include <QtWidgets>
#include "../modulewidget.h"

class MenuWidget : public QWidget
{
public:
    explicit MenuWidget(WidgetType type, QString text, int posX, int posY, int menuWidth, int menuHeight, QString picPath, QColor rgb, QWidget *parent = 0);
    ~MenuWidget();
    QLabel* picture;

    QLabel *getName() const;
    void setName(QLabel *value);

    WidgetType getMenuType() const;
    void setMenuType(const WidgetType &value);

private:
    QLabel* name;
    WidgetType menuType;
    void paintEvent(QPaintEvent *);
};

#endif // MENUWIDGET_H
