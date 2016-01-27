#ifndef VIDEOPLAYERWIDGET_H
#define VIDEOPLAYERWIDGET_H

#include <QtWidgets>
#include <QList>
#include <QMovie>
#include <qmediaplayer.h>
#include "../modulewidget.h"

class VideoplayerWidget : public ModuleWidget
{
    Q_OBJECT
public:
    explicit VideoplayerWidget(QWidget *parent = 0);
    ~VideoplayerWidget();

private:
    QList<QMediaPlayer*> mediaList;

signals:

public slots:
};

#endif // VIDEOPLAYERWIDGET_H
