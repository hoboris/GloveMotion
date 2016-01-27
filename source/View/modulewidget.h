#ifndef MODULEWIDGET_H
#define MODULEWIDGET_H

#include <QtWidgets>
#include <QPair>
#include "../Model/sensordata.h"

enum class WidgetType
{
    noWidget,
    loadingWidget,
    settingsWidget,
    galleryWidget,
    homeWidget,
    videoplayerWidget,
    desktopWidget
};

Q_DECLARE_METATYPE(WidgetType)

class ModuleWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ModuleWidget(WidgetType type, QString text, QColor rgb, QWidget *parent = 0);
    ~ModuleWidget();

    QLabel *getName() const;
    void setName(QLabel *value);

    QColor *getBackgroundColor() const;
    void setBackgroundColor(QColor *value);

    WidgetType getWidgetType() const;
    void setWidgetType(const WidgetType &value);

protected:
    QLabel* name;
    QColor* backgroundColor;
    WidgetType widgetType;
    void paintEvent(QPaintEvent *);

signals:
    void returnHome(WidgetType);

public slots:
    void collapse(QPair<SensorData*,SensorData*>,QPair<SensorData*,SensorData*>,QPair<SensorData*,SensorData*>);
};

#endif // MODULEWIDGET_H
