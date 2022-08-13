#include "resource.h"
#include "ui_resource.h"

Resource::Resource(HttpClass* httpclass, int resid, int restype, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Resource)
{
    ui->setupUi(this);
    this->httpclass = httpclass;
    this->m_id = resid;
    this->m_type = restype;
    initresPage();

    // connect
    connect(ui->backBtn, SIGNAL(clicked()), this, SLOT(backBtn_clicked()));
    connect(ui->replyBtn, SIGNAL(clicked()), this, SLOT(replyBtn_clicked()));
    connect(ui->followBtn, SIGNAL(clicked()), this, SLOT(followBtn_clicked()));
    connect(ui->likeBtn, SIGNAL(clicked()), this, SLOT(likeBtn_clicked()));
    connect(ui->editBtn, SIGNAL(clicked()), this, SLOT(editBtn_clicked()));
    connect(ui->submitBtn, SIGNAL(clicked()), this, SLOT(submitBtn_clicked()));
    connect(ui->deleteBtn, SIGNAL(clicked()), this, SLOT(deleteBtn_clicked()));
}

Resource::~Resource()
{
    delete ui;
}

void Resource::backBtn_clicked()
{
    ui->backBtn->setStyleSheet("QPushButton{border:0px;background:#b84b3e;}");
    QTimer::singleShot(100, this, [=](){
        ui->backBtn->setStyleSheet("QPushButton{border:0px;background:#ed6a5a;}");
    });
    QTimer::singleShot(150, this, [=](){
        this->close();
        emit redirectHomePage();
    });
}

void Resource::initresPage()
{
    // bind grabGesture
    QScroller::grabGesture(ui->resScrollArea->viewport(), QScroller::LeftMouseButtonGesture);

    // init pet page ui
    ui->resScrollArea->setStyleSheet("QWidget{background:#e6ebe0;}");

    this->initrestitleWidget();
    this->initresbodyWidget();
    this->initcommentWidget();
}

void Resource::initrestitleWidget()
{
    ui->headtitleedit->setReadOnly(true);
    this->loadresdata();
    ui->headtitleedit->setText(this->resData["title"]);
    ui->headusernamelabel->setText("@"+this->resData["username"]);
    ui->headtimelabel->setText("release time: " + this->resData["time"]);
    ui->headviewedlabel->setText(this->resData["view_count"] + " viewed");
    ui->headreplylabel->setText(this->resData["reply_count"] + " reply");
    ui->headlikelabel->setText(this->resData["like_count"] + " like");
    ui->submitBtn->setEnabled(false);
    if (this->like_flag == 0)
    {
        QPixmap pixmap(":/res/unlike_icon.png");
        ui->likeBtn->setIcon(pixmap);
    }
    else
    {
        QPixmap pixmap(":/res/like_icon.png");
        ui->likeBtn->setIcon(pixmap);
    }

    if (this->follow_flag == 1)
    {
        QPixmap pixmap(":/res/follow_click_icon.png");
        ui->followBtn->setIcon(pixmap);
    }
    else if (this->follow_flag == 0)
    {
        QPixmap pixmap(":/res/unfollow_click_icon.png");
        ui->followBtn->setIcon(pixmap);
    }
    else
    {
        ui->followBtn->hide();
    }

    if (this->httpclass->m_uid != this->resData["uid"].toInt())
    {
        ui->deleteBtn->hide();
        ui->editBtn->hide();
        ui->submitBtn->hide();
    }
    QLabel *linelabel = new QLabel(ui->restitlewidget);
    linelabel->setFixedSize(411, 2);
    linelabel->setStyleSheet("QLabel{border: 2px solid #5ca4a9;};");
    linelabel->setGeometry(0, 99, 411, 2);
}

void Resource::loadresdata()
{
    this->resData.clear();
    // send get request to resource/<int:rid>
    QString strurl = "resource/" + QString::number(this->m_id) + "/";
    QString strRes = this->httpclass->getrequest(nullptr, 1, strurl);
    QJsonObject objRes = this->httpclass->parseResponse(strRes);

    if (!objRes.empty())
    {
        int code = objRes.take("code").toInt();
        if (code == 200)
        {
            QJsonObject datamap = objRes.value("data").toObject();
            this->resData["title"] = datamap.take("title").toString();
            this->resData["username"] = datamap.take("publisher_username").toString();
            this->resData["time"] = datamap.take("release_time").toString();
            this->resData["view_count"] = QString::number(datamap.take("views_count").toInt());
            this->resData["reply_count"] = QString::number(datamap.take("comments_count").toInt());
            this->resData["like_count"] = QString::number(datamap.take("likes_count").toInt());
            this->resData["media"] = datamap.take("media").toString();
            this->resData["type"] = datamap.take("type").toString();
            this->resData["content"] = datamap.take("content").toString();
            this->resData["uid"] = QString::number(datamap.take("publisher").toInt());
            this->like_flag = objRes.take("like_flag").toInt();
            this->follow_flag = objRes.take("user_follow_flag").toInt();
        }
        else
        {
            QMessageBox::information(this, "Error", "NOT FOUND");
        }
    }
    else
    {
        QMessageBox::information(this, "Error", "NETWORK ERROR");
    }
}

void Resource::replyBtn_clicked()
{
    qDebug() << "reply";
    // new reply
}


void Resource::likeBtn_clicked()
{
//    qDebug() << "like";
    QString strurl = "user/" + QString::number(this->httpclass->m_uid) + "/like/" + QString::number(this->m_id) + "/";
    if (this->like_flag != 1)
    {
        // send post to user/<int:pk>/like/<int:rid>/
        QString strRes = this->httpclass->postrequest(nullptr, 1, strurl);
        QJsonObject objRes = this->httpclass->parseResponse(strRes);
        if (!objRes.empty())
        {
            int code = objRes.take("code").toInt();
            if (code == 200)
            {
                ui->likeBtn->setIcon(QPixmap(":/res/like_icon.png"));
                this->like_flag = 1;
            }
            else
            {
                QMessageBox::information(this, "Error", "NOT FOUND");
            }
        }
        else
        {
            QMessageBox::information(this, "Error", "NETWORK ERROR");
        }
    }
    else
    {
        // send delete to user/<int:pk>/like/<int:rid>/
        this->httpclass->deletequest(1, strurl);
        ui->likeBtn->setIcon(QPixmap(":/res/unlike_icon.png"));
        this->like_flag = 0;
    }
}


void Resource::followBtn_clicked()
{
//    qDebug() << "follow";
    QString strurl = "user/" + QString::number(this->httpclass->m_uid) + "/follow/" + this->resData["uid"] + "/";
    if (this->follow_flag != 1)
    {
        // send post to user/<int:pk>/follow/<int:person>/
        QString strRes = this->httpclass->postrequest(nullptr, 1, strurl);
        QJsonObject objRes = this->httpclass->parseResponse(strRes);
        if (!objRes.empty())
        {
            int code = objRes.take("code").toInt();
            if (code == 200)
            {
                ui->followBtn->setIcon(QPixmap(":/res/follow_click_icon.png"));
                this->follow_flag = 1;
            }
            else
            {
                QMessageBox::information(this, "Error", "NOT FOUND");
            }
        }
        else
        {
            QMessageBox::information(this, "Error", "NETWORK ERROR");
        }
    }
    else
    {
        // send delete to user/<int:pk>/follow/<int:person>/
        this->httpclass->deletequest(1, strurl);
        ui->followBtn->setIcon(QPixmap(":/res/unfollow_click_icon.png"));
        this->follow_flag = 0;
    }
}


void Resource::editBtn_clicked()
{
//    qDebug() << "edit";
    ui->editBtn->setEnabled(false);
    ui->submitBtn->setEnabled(true);
    ui->headtitleedit->setReadOnly(false);

    this->adduploadBtn();
}


void Resource::submitBtn_clicked()
{
    QVBoxLayout *vlayout = ui->resbodywidget->findChild<QVBoxLayout*>();
    if (vlayout == nullptr)
    {
        vlayout = new QVBoxLayout(ui->resbodywidget);
    }
    // set layout
    vlayout->setDirection(QBoxLayout::TopToBottom);
    vlayout->setSpacing(2);
    vlayout->setContentsMargins(10, 10, 10, 10);

    QTextEdit *textedit = ui->resbodywidget->findChild<QTextEdit*>();
    if (textedit != nullptr)
    {
        textedit->setReadOnly(true);
        this->resData["content"] = textedit->toPlainText();
    }
    this->resData["title"] = ui->headtitleedit->text();

    if (this->resData["type"] == "2") // upload photo
    {
        // show new photo
        qDebug() << "show new photo";
    }
    else if (this->resData["type"] == "3") // upload video
    {
        // show new video
        qDebug() << "show new video";
    }
    else
    {
        this->resData["type"] = "1";
    }

    // send put request to user/<int:pk>/resource/<int:rid>/
    QString strurl = "user/" + QString::number(this->httpclass->m_uid) + "/resource/" + QString::number(this->m_id) + "/";
    // create json data
    QJsonObject requestdata;
    requestdata.insert("title", this->resData["title"]);
    requestdata.insert("content", this->resData["content"]);
    requestdata.insert("r_type", this->resData["type"].toInt());

    QString strRes = this->httpclass->putrequest(&requestdata, 1, strurl);
    QJsonObject objRes = this->httpclass->parseResponse(strRes);
    if (!objRes.empty())
    {
        if (!objRes.value("detail").isUndefined())
        {
            QMessageBox::information(this, "Error", objRes.take("detail").toString());
            return;
        }

        int code = objRes.take("code").toInt();
        if (code != 201)
        {
            // handle error
            QMessageBox::information(this, "Error", objRes.take("msg").toString());
            return;
        }
        else
        {
            QMessageBox::information(this, "Success", objRes.take("msg").toString());
            if (this->resData["content"] == "")
            {
                delete textedit;
            }

            QWidget *widget = ui->resbodywidget->findChild<QWidget*>("selectBtns");
            if (widget != nullptr)
            {
                delete widget;
            }
            ui->editBtn->setEnabled(true);
            ui->headtitleedit->setReadOnly(true);
            ui->submitBtn->setEnabled(false);
        }
    }
    else
    {
        QMessageBox::information(this, "Error", "Network Error");
        return;
    }
}


void Resource::deleteBtn_clicked()
{
    // send delete request to user/<int:pk>/resource/<int:pid>/
    QString strurl = "user/" + QString::number(this->httpclass->m_uid) + "/resource/" + QString::number(this->m_id) + "/";
    this->httpclass->deletequest(1, strurl);
    QMessageBox::information(this, "Success", "Delete Success!");
    this->close();
    emit redirectHomePage();
}

void Resource::initresbodyWidget()
{
    QVBoxLayout *vlayout = ui->resbodywidget->findChild<QVBoxLayout*>();
    if (vlayout == nullptr)
    {
        vlayout = new QVBoxLayout(ui->resbodywidget);
    }
    // set layout
    vlayout->setDirection(QBoxLayout::TopToBottom);
    vlayout->setSpacing(2);
    vlayout->setContentsMargins(10, 10, 10, 10);

    if (this->resData["content"] != "")
    {
        QTextEdit *textedit = new QTextEdit(ui->resbodywidget);
        textedit->setText(this->resData["content"]);
        textedit->setFixedWidth(391);
        textedit->setMinimumHeight(200);
        textedit->setStyleSheet("QTextEdit{border: 2px solid;border-radius: 5px groove gray;background-color: rgb(230, 235, 224);}");
        textedit->setReadOnly(true);
        vlayout->addWidget(textedit);
    }
    if (this->resData["media"] != "")
    {
        QString strurl = this->httpclass->m_rootUrl + this->resData["media"].remove(0, 1);
        strurl.remove("/api/v1");
        if (this->resData["type"] == "picture")
        {
            QLabel *label = new QLabel;
            QPixmap pixmap = this->httpclass->downloadphoto(strurl);
            vlayout->addWidget(label);
            pixmap = pixmap.scaled(label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
            label->setPixmap(pixmap);
        }
        else
        {
            QVideoWidget *videoWidget = new QVideoWidget;
            QMediaPlayer *player = new QMediaPlayer;
            videoWidget->resize(391, 220);
            player->setVideoOutput(videoWidget);
//            player->setSource(QUrl("http://127.0.0.1:8000/media/resource/user_11/test.mp4"));
            player->setSource(strurl);
            vlayout->addWidget(videoWidget);
            videoWidget->show();
            QPushButton *btn = new QPushButton(ui->resbodywidget);
            btn->setFixedSize(50,50);
            btn->setGeometry(ui->resbodywidget->width()/2 - btn->width()/2, ui->resbodywidget->height()/2-btn->height()/2, 50, 50);
            btn->setIcon(QPixmap(":/res/play_icon.png"));
            btn->setIconSize(QSize(50,50));
            btn->setStyleSheet("QPushButton{background-color:#000000};");
            connect(btn, &QPushButton::clicked, this, [=](){
                btn->hide();
                player->play();
            },Qt::AutoConnection);
        }
    }

}

void Resource::initcommentWidget()
{
    this->replyData.clear();
    loadreplydata();

    QVBoxLayout *vlayout = ui->commentwidget->findChild<QVBoxLayout*>();
    if (vlayout == nullptr)
    {
        vlayout = new QVBoxLayout(ui->commentwidget);
    }
    // set layout
    vlayout->setDirection(QBoxLayout::TopToBottom);
    vlayout->setSpacing(0);
    vlayout->setContentsMargins(0, 0, 0, 0);

    QLabel *linelabeltop = new QLabel(ui->restitlewidget);
    linelabeltop->setFixedSize(411, 2);
    linelabeltop->setStyleSheet("QLabel{border: 2px solid #5ca4a9;};");
    vlayout->addWidget(linelabeltop);

    for (int i = 0; i < this->replyData.size(); ++i)
    {
//        qDebug() << this->replyData[i];
        QWidget *container = new QWidget(ui->commentwidget);
        QVBoxLayout *containerlayout = new QVBoxLayout;
        containerlayout->setParent(container);
        container->setLayout(containerlayout);
        containerlayout->setDirection(QBoxLayout::TopToBottom);
        containerlayout->setSpacing(2);
        containerlayout->setContentsMargins(6, 10, 10, 10);
        QLabel *usernamelabel = new QLabel;
        QLabel *timelabel = new QLabel;
        QTextEdit *replycontent = new QTextEdit;
        usernamelabel->setText("@" + this->replyData[i]["username"]);
        timelabel->setText(this->replyData[i]["time"]);
        timelabel->setFont(QFont("Microsoft YaHei UI", 6));
        replycontent->setText(this->replyData[i]["content"]);
        replycontent->setReadOnly(true);
        replycontent->setStyleSheet("QTextEdit{border: 2px solid;border-radius: 5px groove gray;background-color: rgb(230, 235, 224);}");
        containerlayout->addWidget(usernamelabel);
        containerlayout->addWidget(timelabel);
        containerlayout->addWidget(replycontent);
        QLabel *linelabel = new QLabel;
        linelabel->setFixedSize(411, 2);
        linelabel->setStyleSheet("QLabel{border: 2px solid #5ca4a9;};");

        vlayout->addWidget(container);
        vlayout->addWidget(linelabel);
    }
    QLabel *endlabel = new QLabel;
    endlabel->setText("No More Reply.");
    vlayout->addWidget(endlabel);

}

void Resource::loadreplydata()
{
    // send get request to resource/<int:pk>/comment/
    int page_num = 1;
    while (1)
    {
        QString strurl = "resource/" + QString::number(this->m_id) + "/comment/?page=" + QString::number(page_num);
        QString strRes = this->httpclass->getrequest(nullptr, 0, strurl);
        QJsonObject objRes = this->httpclass->parseResponse(strRes);
        if (!objRes.empty())
        {
            if (objRes.value("detail").toString() != "")
            {
                return;
            }
            int code = objRes.take("code").toInt();
            if (code == 200)
            {
                QJsonArray datalist = objRes.value("data").toArray();
                for (int i = 0; i < datalist.size(); ++i)
                {
                    QMap<QString, QString> tempmap;
                    tempmap["content"] = datalist[i].toObject().take("content").toString();
                    tempmap["reply_id"] = QString::number(datalist[i].toObject().take("comment_id").toInt());
                    tempmap["time"] = datalist[i].toObject().take("create_time").toString();
                    tempmap["publisher_id"] = QString::number(datalist[i].toObject().take("publisher").toInt());
                    tempmap["username"] = datalist[i].toObject().take("publisher_username").toString();
                    tempmap["rid"] = QString::number(datalist[i].toObject().take("parent").toInt());
                    this->replyData << tempmap;
                }
            }
            else
            {
                QMessageBox::information(this, "Error", "NOT FOUND");
                return;
            }
        }
        else
        {
            QMessageBox::information(this, "Error", "NETWORK ERROR");
            return;
        }
        ++page_num;
    }
}

void Resource::adduploadBtn()
{
    int newlay = 0, newtext = 0, newlab = 0, newvideo = 0;
    QVBoxLayout *vlayout = ui->resbodywidget->findChild<QVBoxLayout*>();
    if (vlayout == nullptr)
    {
        vlayout = new QVBoxLayout(ui->resbodywidget);
        newlay = 1;
    }
    // set layout
    vlayout->setDirection(QBoxLayout::TopToBottom);
    vlayout->setSpacing(2);
    vlayout->setContentsMargins(10, 10, 10, 10);

    QTextEdit *textedit = ui->resbodywidget->findChild<QTextEdit*>();
    if (textedit == nullptr)
    {
        textedit = new QTextEdit(ui->resbodywidget);
        textedit->setFixedWidth(391);
        textedit->setMinimumHeight(200);
        textedit->setStyleSheet("QTextEdit{border: 2px solid;border-radius: 5px groove gray;background-color: rgb(230, 235, 224);}");
        textedit->setReadOnly(false);
        vlayout->addWidget(textedit);
        newtext = 1;
    }
    else
    {
        textedit->setReadOnly(false);
    }

    QLabel *label = ui->resbodywidget->findChild<QLabel*>();
    if (label != nullptr)
    {
        label->hide();
        newlab = 1;
    }

    QVideoWidget *video = ui->resbodywidget->findChild<QVideoWidget*>();
    QPushButton *playbtn = ui->resbodywidget->findChild<QPushButton*>();
    if (video != nullptr)
    {
        video->hide();
        playbtn->hide();
        newvideo = 1;
    }

    // add edit btn widget
    QWidget *container = new QWidget(ui->resbodywidget);
    QHBoxLayout *containerlayout = new QHBoxLayout;
    containerlayout->setParent(container);
    container->setLayout(containerlayout);
    containerlayout->setDirection(QBoxLayout::LeftToRight);
    containerlayout->setAlignment(Qt::AlignLeft);
    containerlayout->setSpacing(2);
    containerlayout->setContentsMargins(0, 10, 0, 10);
    container->setStyleSheet("QPushButton{border-radius: 5px;background-color: rgb(155, 193, 188);border: 1px solid;}");
    QPushButton *pngBtn = new QPushButton;
    QPushButton *videoBtn = new QPushButton;
    QPushButton *backBtn = new QPushButton;
    pngBtn->setFixedSize(20,20);
    videoBtn->setFixedSize(20, 20);
    backBtn->setFixedSize(20,20);
    videoBtn->setIcon(QPixmap(":/res/video_select_icon.png"));
    pngBtn->setIcon(QPixmap(":/res/photo_select_icon.png"));
    backBtn->setIcon(QPixmap(":/res/cancel_icon.png"));
    containerlayout->addWidget(pngBtn);
    containerlayout->addWidget(videoBtn);
    containerlayout->addWidget(backBtn);
    container->setObjectName("selectBtns");
    if (ui->submitBtn->isHidden())
    {
        backBtn->hide();
    }

    QLabel *linelabel = new QLabel;
    linelabel->setFixedSize(391, 2);
    linelabel->setStyleSheet("QLabel{border: 2px solid #5ca4a9;};");

    connect(backBtn, &QPushButton::clicked, this, [=](){
        backBtn->setStyleSheet("QPushButton{border:1px solid;border-radius: 5px;background:#7b9b97;}");
        QTimer::singleShot(100, this, [=](){
            backBtn->setStyleSheet("QPushButton{border:1px solid;border-radius: 5px;background:#9bc1bc;}");
        });
        // cancel edit
        QTimer::singleShot(130, this, [=](){
            if (newtext)
            {
                delete textedit;
            }
            else
            {
                textedit->setReadOnly(true);
            }
            if (newlab)
            {
                label->show();
            }
            if (newvideo)
            {
                video->show();
            }
            if (newlay)
            {
                delete vlayout;
            }
            delete linelabel;
            delete container;
            ui->headtitleedit->setReadOnly(true);
        });
    },Qt::AutoConnection);

    connect(pngBtn, &QPushButton::clicked, this, [=](){
        pngBtn->setStyleSheet("QPushButton{border:1px solid;border-radius: 5px;background:#7b9b97;}");
        QTimer::singleShot(100, this, [=](){
            pngBtn->setStyleSheet("QPushButton{border:1px solid;border-radius: 5px;background:#9bc1bc;}");
        });
        // select photo
        QTimer::singleShot(130, this, [=](){
            qDebug() << "select photo function, send file";
            this->resData["type"] = "2";
        });
    },Qt::AutoConnection);

    connect(videoBtn, &QPushButton::clicked, this, [=](){
        videoBtn->setStyleSheet("QPushButton{border:1px solid;border-radius: 5px;background:#7b9b97;}");
        QTimer::singleShot(100, this, [=](){
            videoBtn->setStyleSheet("QPushButton{border:1px solid;border-radius: 5px;background:#9bc1bc;}");
        });
        // select video
        QTimer::singleShot(130, this, [=](){
            qDebug() << "select video function, send file";
            this->resData["type"] = "3";
        });
    },Qt::AutoConnection);


    vlayout->addWidget(container);
    vlayout->addWidget(linelabel);

}
