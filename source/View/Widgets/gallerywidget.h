#ifndef GALLERYWIDGET_H
#define GALLERYWIDGET_H

#include <QtWidgets>
#include "../modulewidget.h"
#include "../../Model/sensordata.h"
#include <QPair>
class GalleryWidget : public ModuleWidget
{
    Q_OBJECT
public:
    explicit GalleryWidget(QWidget *parent = 0);
    ~GalleryWidget();

    //Stockage des images
    QVector<QPixmap> listeImage;
    QVector<QPixmap> listeImageReduite;

    //Gestion de la visuation des images dans le visionneur
    QMap< int,QLabel*> listeQLabelVisuel;


    int ipp;
    int visu;

    //Gestion du bandeau
    QList<QLabel*> listeQLabelBandeau;
    QGridLayout* layoutBandeau;
    QWidget* widgetBandeauGlobale;
    int milieu;
    int image0;
    int image1;
    int image2;
    int image3;
    int image4;
    int image5;
    int image6;
    int dernierIndiceListeQLabel;
    int valimageAgauche;
    int valimageAdroite;
    QLayoutItem *child;

    //Gestion rotation et deplacement

    int emp_x1;
    int emp_y1;
    int emp_x2;
    int emp_y2;
    int emp_x;
    int emp_y;
    int valx;
    int valy;


    QPixmap p;

    //Selection
    bool thumbCurrent;
    bool thumbSave;
    bool middleCurrent;
    bool middleSave;


    //Fonction
    void paintEvent(QPaintEvent *);
    void chargementImages();
    void initBandeau();
    void majBandeau(int mil);
    void nettoyageLayout();
    void visionneur();
    void rotationDeplacementImage(bool estDansBandeau, int valrotation,int pointeur_x,int pointeur_y);
    void suppressionVisionneur();
    void rechercheImage(int pointeur_x,int pointeur_y);

public slots:
    void displayData(QPair<SensorData*,SensorData*> sensorCurrent, QPair<SensorData*,SensorData*> sensorSave, QPair<SensorData*,SensorData*> sensorZero);

};

#endif // GALLERYWIDGET_H
