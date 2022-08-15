#include "publish.h"
#include "ui_publish.h"

Publish::Publish(HttpClass *httpclass, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Publish)
{
    ui->setupUi(this);

    // init httpclass
    this->httpclass = httpclass;

    // init page
    this->initpublishPage();

    connect(ui->textBtn, SIGNAL(clicked()), this, SLOT(textBtn_clicked()));
    connect(ui->imgBtn, SIGNAL(clicked()), this, SLOT(imgBtn_clicked()));
    connect(ui->videoBtn, SIGNAL(clicked()), this, SLOT(videoBtn_clicked()));
}

Publish::~Publish()
{
    delete ui;
}

void Publish::initpublishPage()
{
    this->setGeometry(0, 1659, 411, 570);
    this->animation = new QPropertyAnimation(this, "geometry");
    // set duration
    this->animation->setDuration(200);
    // set start position
    this->animation->setStartValue(QRect(this->x(),this->y(),this->width(),this->height()));
    // set end position
    this->animation->setEndValue(QRect(this->x(),0,this->width(),this->height()));
}

void Publish::publishPage_Show()
{
    this->animation->start();
}

void Publish::publishPage_Hide()
{
    this->move(0, 1659);
}

void Publish::textBtn_clicked()
{
    ui->textBtn->setStyleSheet("QPushButton{background-color: rgb(102, 102, 102);}");
    QTimer::singleShot(100, this, [=](){
        ui->textBtn->setStyleSheet("QPushButton{background-color: rgba(102, 102, 102, 0);}");
    });
    QTimer::singleShot(130, this, [=](){
        emit publishBtnclicked();
        PublishRes *publishres = new PublishRes(this->httpclass, 1);
        this->publishPage_Hide();
        publishres->show();

        connect(publishres, &PublishRes::redirectHomePage, this, [=](){
            delete publishres;
            emit publishBtnclose();
        }, Qt::AutoConnection);
    });
}


void Publish::imgBtn_clicked()
{
    ui->imgBtn->setStyleSheet("QPushButton{background-color: rgb(102, 102, 102);}");
    QTimer::singleShot(100, this, [=](){
        ui->imgBtn->setStyleSheet("QPushButton{background-color: rgba(102, 102, 102, 0);}");
    });
    QTimer::singleShot(130, this, [=](){
        emit publishBtnclicked();
        PublishRes *publishres = new PublishRes(this->httpclass, 2);
        this->publishPage_Hide();
        publishres->show();

        connect(publishres, &PublishRes::redirectHomePage, this, [=](){
            delete publishres;
            emit publishBtnclose();
        }, Qt::AutoConnection);
    });
}


void Publish::videoBtn_clicked()
{
    ui->videoBtn->setStyleSheet("QPushButton{background-color: rgb(102, 102, 102);}");
    QTimer::singleShot(100, this, [=](){
        ui->videoBtn->setStyleSheet("QPushButton{background-color: rgba(102, 102, 102, 0);}");
    });
    QTimer::singleShot(130, this, [=](){
        emit publishBtnclicked();
        PublishRes *publishres = new PublishRes(this->httpclass, 3);
        this->publishPage_Hide();
        publishres->show();

        connect(publishres, &PublishRes::redirectHomePage, this, [=](){
            delete publishres;
            emit publishBtnclose();
        }, Qt::AutoConnection);
    });
}

