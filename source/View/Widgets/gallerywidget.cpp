#include "gallerywidget.h"
#include <QtWidgets>

GalleryWidget::GalleryWidget(QWidget *parent) : ModuleWidget(WidgetType::galleryWidget,"Gallery",QColor(255,123,6),parent)
{

    //Initialisation des variables
    visu=0;
    widgetBandeauGlobale= new QWidget(this);
    layoutBandeau= new  QGridLayout();


    //Chargement des images dans des listes
    chargementImages();

    //Affichage du bandeau
    initBandeau();
    majBandeau(milieu);

    //Selection
    thumbCurrent=false;
    thumbSave=false;
    middleCurrent=false;
    middleSave=false;
}

void GalleryWidget::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
void GalleryWidget::chargementImages(){
    //Images
    QPixmap pic1normale(":/Resources/phot0.jpg");QPixmap pic2normale(":/Resources/phot1.jpg");QPixmap pic3normale(":/Resources/phot2.jpg");QPixmap pic4normale(":/Resources/phot3.jpg");QPixmap pic5normale(":/Resources/phot4.jpg");QPixmap pic6normale(":/Resources/phot5.jpg");QPixmap pic7normale(":/Resources/phot6.jpg");QPixmap pic8normale(":/Resources/phot7.jpg");QPixmap pic9normale(":/Resources/phot8.jpg");QPixmap pic10normale(":/Resources/phot9.jpg");QPixmap pic11normale(":/Resources/phot10.jpg");

    //Images taille réduite
    QPixmap pic1reduite=pic1normale.scaled(QSize(200,153));QPixmap pic2reduite=pic2normale.scaled(QSize(200,153));QPixmap pic3reduite=pic3normale.scaled(QSize(200,153));QPixmap pic4reduite=pic4normale.scaled(QSize(200,153));QPixmap pic5reduite=pic5normale.scaled(QSize(200,153));QPixmap pic6reduite=pic6normale.scaled(QSize(200,153));QPixmap pic7reduite=pic7normale.scaled(QSize(200,153));QPixmap pic8reduite=pic8normale.scaled(QSize(200,153));QPixmap pic9reduite=pic9normale.scaled(QSize(200,153));QPixmap pic10reduite=pic10normale.scaled(QSize(200,153));QPixmap pic11reduite=pic11normale.scaled(QSize(200,153));

    //Liste image taille normale
    listeImage.push_back(pic1normale);listeImage.push_back(pic2normale);listeImage.push_back(pic3normale);listeImage.push_back(pic4normale);listeImage.push_back(pic5normale);listeImage.push_back(pic6normale);listeImage.push_back(pic7normale);listeImage.push_back(pic8normale);listeImage.push_back(pic9normale);listeImage.push_back(pic10normale);listeImage.push_back(pic11normale);

    //Liste image taille réduite pour bandeau
    listeImageReduite.push_back(pic1reduite);listeImageReduite.push_back(pic2reduite);listeImageReduite.push_back(pic3reduite);listeImageReduite.push_back(pic4reduite);listeImageReduite.push_back(pic5reduite);listeImageReduite.push_back(pic6reduite);listeImageReduite.push_back(pic7reduite);listeImageReduite.push_back(pic8reduite);listeImageReduite.push_back(pic9reduite);listeImageReduite.push_back(pic10reduite);listeImageReduite.push_back(pic11reduite);
}
void GalleryWidget::initBandeau(){
    //Liste de label pour le bandeau
    for (int i=0;i<listeImageReduite.length();i++){
        QLabel* temporaire =new QLabel();
        temporaire->setPixmap(listeImageReduite[i]);
        listeQLabelBandeau << temporaire;
    }
    milieu=(int)(listeQLabelBandeau.size()/2.0);

}
void GalleryWidget::majBandeau(int mil){
    milieu=mil;
    visu=milieu;
    //Widget du bandeau
    widgetBandeauGlobale->setAutoFillBackground(true);
    widgetBandeauGlobale->setStyleSheet("background-color:rgb(68,68,68);");
    widgetBandeauGlobale->setGeometry(
                ((int) (this->width()*0.14))
                ,((int) (this->height()*0.8))
                ,((int) (this->width()*0.72))
                ,(int) (this->height()*0.21));
    //Nettoyage du Layout
    QLayoutItem *child;
    while ((child = layoutBandeau->takeAt(0)) != 0) {
        layoutBandeau->removeItem(child);
        delete child;
    }

    //Stockage des images dans le layout
    dernierIndiceListeQLabel=listeQLabelBandeau.size()-1;
    valimageAgauche=milieu;
    valimageAdroite=milieu;
    if (milieu==2){
        image1=valimageAgauche-2;
        image2=valimageAgauche-1;
        image3=milieu;
        valimageAgauche=dernierIndiceListeQLabel;
        image0=valimageAgauche;
    }else if (milieu==1){
        image2=valimageAgauche-1;
        image3=milieu;
        valimageAgauche=dernierIndiceListeQLabel;
        image0=valimageAgauche-1;
        image1=valimageAgauche;
    }else if (milieu==0){
        valimageAgauche=dernierIndiceListeQLabel;
        image0=valimageAgauche-2;
        image1=valimageAgauche-1;
        image2=valimageAgauche;
        image3=milieu;
    }else if(milieu>=3){
        image0=valimageAgauche-3;
        image1=valimageAgauche-2;
        image2=valimageAgauche-1;
        image3=milieu;
    }

    if (milieu==dernierIndiceListeQLabel){
        valimageAdroite=0;
        image4=valimageAdroite;
        image5=valimageAdroite+1;
        image6=valimageAdroite+2;
    }else if (milieu+1==dernierIndiceListeQLabel){
        image4=valimageAdroite+1;
        valimageAdroite=0;
        image5=valimageAdroite;
        image6=valimageAdroite+1;
    }else if (milieu+2==dernierIndiceListeQLabel){
        image4=valimageAdroite+1;
        image5=valimageAdroite+2;
        valimageAdroite=0;
        image6=valimageAdroite;
    }else if(milieu+3<=dernierIndiceListeQLabel){//Cas ok
        image4=valimageAdroite+1;
        image5=valimageAdroite+2;
        image6=valimageAdroite+3;
    }

    layoutBandeau->addWidget(listeQLabelBandeau[image0],7,0);
    layoutBandeau->addWidget(listeQLabelBandeau[image1],7,1);
    layoutBandeau->addWidget(listeQLabelBandeau[image2],7,2);
    layoutBandeau->addWidget(listeQLabelBandeau[image3],0,3,15,4);
    layoutBandeau->addWidget(listeQLabelBandeau[image4],7,7);
    layoutBandeau->addWidget(listeQLabelBandeau[image5],7,8);
    layoutBandeau->addWidget(listeQLabelBandeau[image6],7,9);

    //Mise en place du layout sur le widget
    widgetBandeauGlobale->setLayout(layoutBandeau);
    widgetBandeauGlobale->raise();
}
void GalleryWidget::rechercheImage(int pointeur_x,int pointeur_y){
    ipp=-50;
    if (!listeQLabelVisuel.isEmpty()){
        QMap<int,QLabel*>::const_iterator iterator = listeQLabelVisuel.constBegin();
        while (iterator != listeQLabelVisuel.constEnd()) {
            emp_x1=iterator.value()->x();
            emp_x2=iterator.value()->x()+iterator.value()->width();
            emp_y1=iterator.value()->y();
            emp_y2=iterator.value()->y()+iterator.value()->height();
            if ((pointeur_x>emp_x1) && (pointeur_x<emp_x2) && (pointeur_y>emp_y1) && (pointeur_y<emp_y2) ){
                p=listeImage[iterator.key()];
                valx=pointeur_x-iterator.value()->x();
                valy=pointeur_y-iterator.value()->y();
                ipp=iterator.key();
            }
            ++iterator;
        }
    }
}

void GalleryWidget::rotationDeplacementImage(bool estDansBandeau, int valrotation,int pointeur_x,int pointeur_y){
    if (ipp!=-50 || estDansBandeau){

        if (ipp!=-50) {
            listeQLabelVisuel[ipp]->setVisible(false);

        }
        if (estDansBandeau &&ipp==-50){
            ipp=visu;
            emp_x=500;
            emp_y=500;
            valx=listeImage[visu].width()/2.0;
            valy=listeImage[visu].height()/2.0;
        }

        QTransform qt;
        qt.rotate(valrotation);
        QPixmap imageTemp=p.transformed(qt);
        QLabel* tempo= new QLabel(this);
        listeQLabelVisuel.insert(ipp,tempo);
        listeQLabelVisuel[ipp]->setPixmap(imageTemp);
        listeQLabelVisuel[ipp]->setGeometry(
                    emp_x
                    ,emp_y
                    ,imageTemp.width()
                    ,imageTemp.height());
        listeQLabelVisuel[ipp]->move(pointeur_x-valx,pointeur_y-valy);
        listeQLabelVisuel[ipp]->setStyleSheet("background:transparent;");
        listeQLabelVisuel[ipp]->setVisible(true);
        majBandeau(visu);
    }

}

void GalleryWidget::suppressionVisionneur(){
    if (ipp!=-50){
        listeQLabelVisuel[ipp]->move(4000,4000);
    }
}
void GalleryWidget::displayData(QPair<SensorData*,SensorData*> sensorCurrent, QPair<SensorData*,SensorData*> sensorSave, QPair<SensorData*,SensorData*> sensorZero)
{
    //Une selection pouce et majeur

    if(this->isVisible()){

        QPair<int,int> pointCurrent(sensorCurrent.first->calculatePos(sensorZero.first,this->width(),this->height()));
        QPair<int,int> pointSave(sensorSave.first->calculatePos(sensorZero.first,this->width(),this->height()));
        if (sensorCurrent.first->isCorrect()){


            thumbCurrent=sensorCurrent.first->getThumb()->isSelected();

            // Viens d'être sélectionné
            if (thumbCurrent && !thumbSave){
                thumbSave = thumbCurrent;
                rechercheImage(pointCurrent.first,pointCurrent.second);
            }
            // Viens d'être désélectionné
            else if (!thumbCurrent && thumbSave){
                // Enlever image stockée
                thumbSave = thumbCurrent;
            }
            // Est sélectionné
            else if (thumbCurrent && thumbSave){
                if (sensorCurrent.first->isCorrect() && sensorCurrent.first->getMiddle()->isSelected() && !sensorCurrent.first->getIndex()->isSelected()){

                    //Le pointeur est dans le bandeau
                    bool estDansBandeauSave=false;
                    if ((widgetBandeauGlobale->x() < pointSave.first)&&
                            ((widgetBandeauGlobale->x()+widgetBandeauGlobale->width()) > pointSave.first) &&
                            (widgetBandeauGlobale->y() < pointSave.second)&&
                            ((widgetBandeauGlobale->y()+widgetBandeauGlobale->height()) > pointSave.second) ){
                        estDansBandeauSave=true;
                    }

                    //Le pointeur est dans le bandeau
                    bool estDansBandeauCurrent=false;
                    if ((widgetBandeauGlobale->x() < pointCurrent.first)&&
                            ((widgetBandeauGlobale->x()+widgetBandeauGlobale->width()) > pointCurrent.first) &&
                            (widgetBandeauGlobale->y() < pointCurrent.second)&&
                            ((widgetBandeauGlobale->y()+widgetBandeauGlobale->height()) > pointCurrent.second) ){
                        estDansBandeauCurrent=true;
                    }


                    bool rollDown;
                    rollDown=(sensorCurrent.first->getHand()->getRoll()-sensorSave.first->getHand()->getRoll())>20;
                    bool rollUp;
                    rollUp=(sensorCurrent.first->getHand()->getRoll()-sensorSave.first->getHand()->getRoll())<-20;

                    float pitch= sensorCurrent.first->getHand()->getPitch()-sensorSave.first->getHand()->getPitch();

                    //Monter image sur le visionneur
                    if (estDansBandeauSave && rollUp){
                        if (!listeQLabelVisuel.contains(visu)){
                            p=listeImage[visu];
                        }

                            rotationDeplacementImage(estDansBandeauSave,(int) pitch,pointCurrent.first,pointCurrent.second);

                    }
                    else if (rollDown && estDansBandeauCurrent){
                        rechercheImage(pointCurrent.first,pointCurrent.second);
                        suppressionVisionneur();
                    }else if (!estDansBandeauSave && !estDansBandeauCurrent) {
                        //Deplacer image
                        rotationDeplacementImage(estDansBandeauSave,(int) pitch,pointCurrent.first,pointCurrent.second);
                        this->setStyleSheet("background-color:rgb(255,123,6)");
                        repaint();

                    }

                }
                // Seulement si et seulement si le pouce est selectionné
                else if (sensorCurrent.first->isCorrect() && !sensorCurrent.first->getMiddle()->isSelected() && !sensorCurrent.first->getIndex()->isSelected()){
                    if (pointSave.first - pointCurrent.first < - this->width() / 8){
                        sensorSave.first->setArm(sensorCurrent.first->getArm());
                        sensorSave.first->setHand(sensorCurrent.first->getHand());
                        if (milieu>0){
                            milieu=milieu-1;
                        }else if(milieu<=0){
                            milieu=listeImage.size()-1;
                        }
                        majBandeau(milieu);
                    }
                    else if (pointSave.first - pointCurrent.first > this->width() / 8){
                        sensorSave.first->setArm(sensorCurrent.first->getArm());
                        sensorSave.first->setHand(sensorCurrent.first->getHand());
                        if (milieu<listeImage.size()-1){
                            milieu=milieu+1;
                        }else if(milieu>=listeImage.size()-1){
                            milieu=0;
                        }
                        majBandeau(milieu);
                    }
                }
            }
        }
        // Est désélectionné
        else if (!thumbCurrent && !thumbSave){
            ipp=-50;
        }
    }
}

GalleryWidget::~GalleryWidget()
{
    for (int i = 0; i < listeQLabelBandeau.size(); i++)
        delete listeQLabelBandeau[i];
    delete layoutBandeau;
    delete widgetBandeauGlobale;
}
