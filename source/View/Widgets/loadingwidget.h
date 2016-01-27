#ifndef LOADINGWIDGET_H
#define LOADINGWIDGET_H

#include <QtWidgets>
#include "../modulewidget.h"

class LoadingWidget : public ModuleWidget
{
    Q_OBJECT
public:
    explicit LoadingWidget(QWidget *parent = 0);
    ~LoadingWidget();

private:
    int splashCounter;
    QLabel* splash;
    QLabel* subtitle;
    QVector<QPixmap> pixmaps;

signals:

public slots:
    void updateSplash();
    void updateSubtitle(QString);
};

#endif // LOADINGWIDGET_H
