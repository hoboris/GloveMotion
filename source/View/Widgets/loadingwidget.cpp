#include "loadingwidget.h"

LoadingWidget::LoadingWidget(QWidget *parent) : ModuleWidget(WidgetType::loadingWidget,"",QColor(0,128,128),parent)
{
    pixmaps.push_back(QPixmap(":/Resources/loading_1.png"));
    pixmaps.push_back(QPixmap(":/Resources/loading_2.png"));
    pixmaps.push_back(QPixmap(":/Resources/loading_3.png"));
    pixmaps.push_back(QPixmap(":/Resources/loading_4.png"));
    splash = new QLabel(this);
    splash->setGeometry((this->width()/2)-(pixmaps[0].width()/2),(this->height()/2)-(pixmaps[0].height()/2),pixmaps[0].width(),pixmaps[0].height());
    splash->setPixmap(pixmaps[0]);

    QLabel* title = new QLabel("Glove Motion",this);
    title->setStyleSheet("color:white;");
    title->setFont(QFont("Segoe UI Light",72));
    title->move((this->width()/2)-(title->fontMetrics().width(title->text())/2),100);

    subtitle = new QLabel("Welcome !",this);
    subtitle->setStyleSheet("color:white;qproperty-alignment: AlignCenter;");
    subtitle->setFont(QFont("Segoe UI Light",24));
    subtitle->setGeometry((this->width()/2)-(subtitle->fontMetrics().width(subtitle->text())/2),splash->y()+splash->height()+50,subtitle->fontMetrics().width(subtitle->text()),subtitle->fontMetrics().height());

    splashCounter = 0;
    QTimer *splashTimer = new QTimer(this);
    connect(splashTimer, SIGNAL(timeout()), this, SLOT(updateSplash()));
    splashTimer->start(500);
}

LoadingWidget::~LoadingWidget()
{
    delete splash;
    delete subtitle;
}

void LoadingWidget::updateSplash()
{
    if (isVisible()){
        splashCounter++;
        splash->setPixmap(pixmaps[splashCounter%4]);
    }
}

void LoadingWidget::updateSubtitle(QString text){
    if (isVisible()){
        subtitle->setText(text);
        subtitle->setGeometry((this->width()/2)-(subtitle->fontMetrics().width(subtitle->text())/2),splash->y()+splash->height()+50,subtitle->fontMetrics().width(subtitle->text()),subtitle->fontMetrics().height());
    }
}

/*QTransform transform;
transform.rotate(0);
transform.scale(0.5,0.5);
*pixmap = pixmap->transformed(transform);*/
