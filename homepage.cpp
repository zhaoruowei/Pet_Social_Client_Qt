#include "homepage.h"
#include "ui_homepage.h"

HomePage::HomePage(QSize screensize, HttpClass *httpclass, UserDB *userdb, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::HomePage)
{
    // init db
    this->userdb = userdb;

    // init http class
    this->httpclass = httpclass;

    // init ui
    ui->setupUi(this);

    // init size(width=411, height=659)
    this->m_screenSize = screensize;

    // init login page
    this->userloginPage = new UserLogin(this->httpclass, this->userdb);
    // init signup page
    this->usersignupPage = new UserSignup(this->httpclass);
    // init resetpwd page
    this->resetpwdPage = new ResetPwd(this->httpclass);
    // init user res, comment page
    this->userresPage = new UserResource(this->httpclass);
    // init publish page
    this->publishPage = new Publish(this->httpclass, this);

    // check login status
    if (!this->httpclass->isLogin(this->httpclass->m_uid, this->httpclass->m_token))
    {
        // show login page
        this->userloginPage->show();
    }
    else
    {
        this->show();
    }

    // init homepage
    this->initHomePage();
    this->initFollowPage();
    this->initPetPage();
    this->initProfilePage();

    // connect redirect to homepage signal
    connect(this->userloginPage, SIGNAL(redirectHomePage()), this, SLOT(redirect_HomePage()));
    connect(this->resetpwdPage, SIGNAL(redirectHomePage()), this, SLOT(redirect_HomePage()));
    connect(this->userresPage, SIGNAL(redirectHomePage()), this, SLOT(redirect_HomePage()));

    // connect redirect to signuppage signal
    connect(this->userloginPage, SIGNAL(redirectSignupPage()), this, SLOT(redirect_SignupPage()));

    // connect redirect to login signal
    connect(this->usersignupPage, SIGNAL(redirectLoginPage()), this, SLOT(redirect_LoginPage()));

    // connect footnav btn click signal
    connect(ui->homeBtn, SIGNAL(clicked()), this, SLOT(homeBtn_clicked()));
    connect(ui->followBtn, SIGNAL(clicked()), this, SLOT(followBtn_clicked()));
    connect(ui->publishBtn, SIGNAL(clicked()), this, SLOT(publishBtn_clicked()));
    connect(ui->petBtn, SIGNAL(clicked()), this, SLOT(petBtn_clicked()));
    connect(ui->mineBtn, SIGNAL(clicked()), this, SLOT(mineBtn_clicked()));
    // connect headnav btn click signal
    connect(ui->recommendBtn, SIGNAL(clicked()), this, SLOT(recommendBtn_clicked()));
    connect(ui->followheadBtn, SIGNAL(clicked()), this, SLOT(followheadBtn_clicked()));

}

HomePage::~HomePage()
{
    delete ui;
}

// redirect signal
void HomePage::redirect_HomePage()
{
    this->setGeometry(this->userloginPage->geometry());
    this->userloginPage->hide();
    this->resetpwdPage->hide();
    this->userresPage->hide();
    this->show();
}
void HomePage::redirect_SignupPage()
{
    this->usersignupPage->setGeometry(this->userloginPage->geometry());
    this->userloginPage->hide();
    this->usersignupPage->show();
}
void HomePage::redirect_LoginPage()
{
    this->userloginPage->setGeometry(this->geometry());
    this->usersignupPage->hide();
    this->userloginPage->show();
}

// init homepage
void HomePage::initHomePage()
{
    // for test android
    this->setFixedSize(QSize(411, 659));
    //    this->setFixedSize(this->m_screenSize);

    // set stack page
    ui->stackedWidget->setCurrentIndex(0);

    // init footnav
    this->initNav();
    this->initFootNavBtn();
    // init headnav
    this->initHeadNavBtn();

    // init scrollarea(request to url: resource/)
    this->m_type = "explore";
    this->initContentSctollarea();

}

// init follow page
void HomePage::initFollowPage()
{
    QPixmap pixmap;
    QString strpath = ":/res/follow_icon.png";
    bool ret = pixmap.load(strpath);
    if (!ret)
    {
        qDebug() << strpath << "load icon fail！";
    }
    ui->followpageheadBtn->setStyleSheet("QPushButton{border:0px;background:#ed6a5a;}");
    ui->followpageheadBtn->setIcon(pixmap);
    ui->followpageheadBtn->setIconSize(QSize(60, 60));
}

// init pet page
void HomePage::initPetPage()
{
    QPixmap pixmap;
    QString strpath = ":/res/cat_icon.png";
    bool ret = pixmap.load(strpath);
    if (!ret)
    {
        qDebug() << strpath << "load icon fail！";
    }
    ui->petpageheadBtn->setStyleSheet("QPushButton{border:0px;background:#ed6a5a;}");
    ui->petpageheadBtn->setIcon(pixmap);
    ui->petpageheadBtn->setIconSize(QSize(60, 60));
}

// init Profile Page
void HomePage::initProfilePage()
{
    QPixmap pixmap;
    QString strpath = ":/res/mine_icon.png";
    bool ret = pixmap.load(strpath);
    if (!ret)
    {
        qDebug() << strpath << "load icon fail！";
    }
    ui->minepageheadBtn->setStyleSheet("QPushButton{border:0px;background:#ed6a5a;}");
    ui->minepageheadBtn->setIcon(pixmap);
    ui->minepageheadBtn->setIconSize(QSize(60, 60));
    ui->bodysubmitBtn->hide();
    ui->bodycancelBtn->hide();

    connect(ui->avatarBtn, SIGNAL(clicked()), this, SLOT(avatarBtn_clicked()));
    connect(ui->headeditBtn, SIGNAL(clicked()), this, SLOT(headeditBtn_clicked()));
    connect(ui->bodysubmitBtn, SIGNAL(clicked()), this, SLOT(bodysubmitBtn_clicked()));
    connect(ui->bodycancelBtn, SIGNAL(clicked()), this, SLOT(bodycancelBtn_clicked()));
    connect(ui->footlogoutBtn, SIGNAL(clicked()), this, SLOT(footlogoutBtn_clicked()));
    connect(ui->footresetBtn, SIGNAL(clicked()), this, SLOT(footresetBtn_clicked()));
    connect(ui->footuserresBtn, SIGNAL(clicked()), this, SLOT(footuserresBtn_clicked()));
    connect(ui->footusercommentBtn, SIGNAL(clicked()), this, SLOT(footusercommentBtn_clicked()));
}

// init navbtn
void HomePage::initNav()
{
    ui->footnav->setStyleSheet("QWidget{background:#ed6a5a;border:0px solid #292929;}");
    ui->headnav->setStyleSheet("QWidget{background:#ed6a5a;border:0px solid #292929;}");
}

void HomePage::initFootNavBtn()
{
    this->footbtns << ui->homeBtn << ui->followBtn << ui->publishBtn << ui->petBtn << ui->mineBtn;
    QList<QString> stricons;
    stricons << "home" << "follow" << "publish" << "cat" << "mine";
    for (int i = 0; i < this->footbtns.size(); ++i)
    {
        QPixmap pixmap;
        QString strpath = ":/res/" + stricons.at(i) + "_icon.png";
        bool ret = pixmap.load(strpath);
        if (!ret)
        {
            qDebug() << strpath << "load icon fail！";
        }
        this->footbtns.at(i)->setFixedSize(83, 89);
        this->footbtns.at(i)->setStyleSheet("QPushButton{border:0px;}");
        this->footbtns.at(i)->setIcon(pixmap);
        this->footbtns.at(i)->setIconSize(QSize(50, 50));
    }
    footbtnCheck();
}
// init headnav
void HomePage::initHeadNavBtn()
{
    this->headbtns << ui->recommendBtn << ui->followheadBtn;
    QList<QString> stricons;
    stricons << "explore" << "follow";
    for (int i = 0; i < this->headbtns.size(); ++i)
    {
        QPixmap pixmap;
        QString strpath = ":/res/" + stricons.at(i) + "_head_icon.png";
        bool ret = pixmap.load(strpath);
        if (!ret)
        {
            qDebug() << strpath << "load icon fail！";
        }
        this->headbtns.at(i)->setFixedSize(203, 89);
        this->headbtns.at(i)->setStyleSheet("QPushButton{border:0px;}");
        this->headbtns.at(i)->setIcon(pixmap);
        this->headbtns.at(i)->setIconSize(QSize(25, 25));
    }
    headbtnCheck();
}

void HomePage::initContentSctollarea()
{
    if (this->m_resourcepage == -1)
    {
        return;
    }
    this->m_resourcepage = 1;

    if (this->m_type == "explore")
    {
        this->loadrecommendcontent();
    }
    else
    {
        this->loadlikecontent();
    }


    // bind grabgesture
    QScroller::grabGesture(ui->contentScrollArea->viewport(), QScroller::LeftMouseButtonGesture);

    // set layout
    QVBoxLayout *vlayout = new QVBoxLayout(ui->contentScrollAreaContent);
    vlayout->setDirection(QBoxLayout::TopToBottom);
    vlayout->setSpacing(0);
    vlayout->setContentsMargins(0, 0, 0, 0);
    // bind widget
    ui->contentScrollAreaContent->setLayout(vlayout);

    this->addresourcebtn();

    // new page content
    connect(ui->contentScrollArea->verticalScrollBar(), &QScrollBar::valueChanged, this, [=](int val){
        if (ui->contentScrollArea->verticalScrollBar()->minimum() == val)
        {
            this->cleanbtn();
            this->m_resourcepage = 1;
            if (this->m_type == "explore")
            {
                this->loadrecommendcontent();
            }
            else
            {
                this->loadlikecontent();
            }
            this->addresourcebtn();
        }
        else if (ui->contentScrollArea->verticalScrollBar()->maximum() == val)
        {
            if (this->m_resourcepage == -1)
            {
                return;
            }
            this->m_resourcepage++;
            if (this->m_type == "explore")
            {
                this->loadrecommendcontent();
            }
            else
            {
                this->loadlikecontent();
            }
            this->addresourcebtn();
        }
    }, Qt::AutoConnection);
}

void HomePage::loadrecommendcontent()
{
    this->resData.clear();
    // to resource/?page=1
    QString strurl = "resource/?page=" + QString::number(this->m_resourcepage);
    QString res = this->httpclass->getrequest(nullptr, 0, strurl);

    // pares response
    QJsonObject objRes = this->httpclass->parseResponse(res);
    this->detail = "";

    if (!objRes.empty())
    {
        if (!objRes.value("detail").isUndefined())
        {
            this->detail = objRes.take("detail").toString();
        }

        int code = objRes.take("code").toInt();
        if (code == 200)
        {
            QJsonArray datalist = objRes.value("data").toArray();
            for (int i = 0; i < datalist.size(); ++i)
            {
                QMap<QString, QString> tempmap;
                tempmap["title"] = datalist[i].toObject().take("title").toString();
                tempmap["id"] = QString::number(datalist[i].toObject().take("resource_id").toInt());
                this->resData << tempmap;
            }
        }
    }
}
// load homepage like content
void HomePage::loadlikecontent()
{
    // to user/<int:pk>/like/?page=1
    QString strurl = "user/" +  QString::number(this->httpclass->m_uid) + "/like/?page=" + QString::number(this->m_resourcepage);
    QString res = this->httpclass->getrequest(nullptr, 1, strurl);

    // pares response
    QJsonObject objRes = this->httpclass->parseResponse(res);
    this->detail = "";

    if (!objRes.empty())
    {
        if (!objRes.value("detail").isUndefined())
        {
            this->detail = objRes.take("detail").toString();
        }

        int code = objRes.take("code").toInt();
        if (code == 200)
        {
            QJsonArray datalist = objRes.value("data").toArray();
            for (int i = 0; i < datalist.size(); ++i)
            {
                QMap<QString, QString> tempmap;
                tempmap["title"] = datalist[i].toObject().take("title").toString();
                tempmap["id"] = QString::number(datalist[i].toObject().take("resource_id").toInt());
                this->resData << tempmap;
            }
        }
    }
}

void HomePage::addresourcebtn()
{
    QVBoxLayout *vlayout = ui->contentScrollAreaContent->findChild<QVBoxLayout*>();
    if (vlayout == nullptr)
    {
        vlayout = new QVBoxLayout(ui->contentScrollAreaContent);
    }
    // set layout
    vlayout->setDirection(QBoxLayout::TopToBottom);
    vlayout->setSpacing(0);
    vlayout->setContentsMargins(0, 0, 0, 0);

    // insert widget
    if (this->detail != "")
    {
        this->m_resourcepage = -1;
        QLabel *label = new QLabel(this);
        label->setText("No More Content.");
        label->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        vlayout->addWidget(label);
    }
    else
    {
        for (int i = 0; i < this->resData.size(); ++i)
        {
            QPushButton *btn = new QPushButton;
            btn->setParent(this);
            btn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
            btn->setText(this->resData[i]["title"]);
            btn->setObjectName(this->resData[i]["id"]);
            QPixmap pixmap;
            pixmap.load(":/res/resource_icon.png");
            btn->setFixedSize(411, 150);
            btn->setStyleSheet("QPushButton{border:1px solid #9bc1bc;background:#e6ebe0;}");
            btn->setIcon(pixmap);
            btn->setIconSize(QSize(60, 60));
            btn->setFont(QFont("Microsoft YaHei UI", 13));
            vlayout->addWidget(btn);

            connect(btn, &QPushButton::clicked, this, [=](){
                int rid = btn->objectName().remove("/").toInt();
                this->resourcePage = new Resource(this->httpclass, rid, 1);
                this->resourcePage->show();
                this->hide();
                connect(this->resourcePage, &Resource::redirectHomePage, this, [=](){
                    delete this->resourcePage;
                    this->resourcePage = nullptr;
                    this->show();
                }, Qt::AutoConnection);
            }, Qt::AutoConnection);
        }
    }
}



void HomePage::initFollowSctollarea()
{
    // get request to /api/v1/user/<int:pk>/follow/
    QString res = this->httpclass->getrequest(nullptr, 1, "user/"+QString::number(this->httpclass->m_uid) + "/follow/");

    // pares response
    QJsonObject objRes = this->httpclass->parseResponse(res);
    QString detail = "";
    QList<QMap<QString, QString>> datamap;
    if (!objRes.empty())
    {
        if (!objRes.value("detail").isUndefined())
        {
            detail = objRes.take("detail").toString();
        }

        int code = objRes.take("code").toInt();
        if (code == 200)
        {
            QJsonArray datalist = objRes.value("data").toArray();
            for (int i = 0; i < datalist.size(); ++i)
            {
                QMap<QString, QString> tempmap;
                tempmap["name"] = datalist[i].toObject().take("name").toString();
                tempmap["gender_display"] = datalist[i].toObject().take("gender_display").toString();
                tempmap["age"] = datalist[i].toObject().take("age").toString();
                tempmap["email"] = datalist[i].toObject().take("email").toString();
                tempmap["phone"] = datalist[i].toObject().take("phone").toString();
                datamap << tempmap;
            }
        }
    }
    // bind grabgesture
    //    QScroller::grabGesture(ui->contentScrollArea, QScroller::LeftMouseButtonGesture);
    QScroller::grabGesture(ui->followScrollArea->viewport(), QScroller::LeftMouseButtonGesture);
    // set layout
    QVBoxLayout *vlayout = new QVBoxLayout(ui->followScrollAreaContent);
    vlayout->setDirection(QBoxLayout::TopToBottom);
    vlayout->setSpacing(0);
    vlayout->setContentsMargins(0, 0, 0, 0);

    // insert widget
    for (int i = 0; i < datamap.size(); ++i)
    {
        QPushButton *btn = new QPushButton;
        btn->setParent(this);
        btn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
        btn->setText(datamap[i]["name"]);
        QPixmap pixmap;
        pixmap.load(":/res/friend_icon.png");
        btn->setFixedSize(411,150);
        btn->setStyleSheet("QPushButton{border:1px solid #9bc1bc;background:#e6ebe0;}");
        btn->setIcon(pixmap);
        btn->setIconSize(QSize(60, 60));
        btn->setFont(QFont("Microsoft YaHei UI", 13));
        btn->setCheckable(true);
        vlayout->addWidget(btn);
        QWidget *btnwidget = new QWidget;
        btnwidget->setParent(this);
        btnwidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
        btnwidget->setFixedSize(411, 150);
        btnwidget->setVisible(false);
        vlayout->addWidget(btnwidget);
        QVBoxLayout *btnwlayout = new QVBoxLayout;
        btnwidget->setLayout(btnwlayout);
        btnwidget->setStyleSheet("QWidget{background:#f4f1bb;}");
        QLabel *label1 = new QLabel(btnwidget);
        QLabel *label2 = new QLabel(btnwidget);
        QLabel *label3 = new QLabel(btnwidget);
        QLabel *label4 = new QLabel(btnwidget);
        QLabel *label5 = new QLabel(btnwidget);
        label1->setText("Name: "+datamap[i]["name"]);
        label2->setText("Gender: "+datamap[i]["gender_display"]);
        label3->setText("Age: "+datamap[i]["age"]);
        label4->setText("Email: "+datamap[i]["email"]);
        label5->setText("Phone: "+datamap[i]["phone"]);
        btnwlayout->addWidget(label1);
        btnwlayout->addWidget(label2);
        btnwlayout->addWidget(label3);
        btnwlayout->addWidget(label4);
        btnwlayout->addWidget(label5);

        connect(btn, &QPushButton::clicked, this, [=](){
            if(btn->isChecked() == true)
            {
                btnwidget->setVisible(true);
            }
            else
            {
                btnwidget->setVisible(false);
            }

        }, Qt::AutoConnection);
    }

    ui->followScrollAreaContent->setLayout(vlayout);
}

// init pet page scrollarea
void HomePage::initPetSctollarea()
{
    // get request to /api/v1/user/<int:pk>/pet/
    QString res = this->httpclass->getrequest(nullptr, 1, "user/"+QString::number(this->httpclass->m_uid) + "/pet/");
    // pares response
    QJsonObject objRes = this->httpclass->parseResponse(res);
    QList<QMap<QString, QString>> datamap;
    if (!objRes.empty())
    {
        int code = objRes.take("code").toInt();
        if (code == 200)
        {
            QJsonArray datalist = objRes.value("data").toArray();
            for (int i = 0; i < datalist.size(); ++i)
            {
                QMap<QString, QString> tempmap;
                tempmap["pet_name"] = datalist[i].toObject().take("pet_name").toString();
                tempmap["pet_id"] = QString::number(datalist[i].toObject().take("pet_id").toInt());
                datamap << tempmap;
            }
        }
    }

    // bind grabGesture
    QScroller::grabGesture(ui->petScrollArea->viewport(), QScroller::LeftMouseButtonGesture);

    // init pet page ui
    ui->petScrollArea->setStyleSheet("QWidget{background:#e6ebe0;}");

    // set layout
    QVBoxLayout *vlayout = new QVBoxLayout(ui->petScrollAreaContent);
    vlayout->setDirection(QBoxLayout::TopToBottom);
    vlayout->setSpacing(0);
    vlayout->setContentsMargins(0, 1, 0, 1);

    // insert widget
    for (int i = 0; i < datamap.size(); ++i)
    {
        // insert btn
        QPushButton *btn = new QPushButton;
        btn->setParent(ui->petScrollAreaContent);
        btn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
        btn->setText(datamap[i]["pet_name"]);
        QPixmap pixmap;
        pixmap.load(":/res/cat_icon.png");
//        btn->setFixedWidth(411);
        btn->setFixedSize(411, 100);
        btn->setStyleSheet("QPushButton{border:1px solid #5ca4a9;background:#9bc1bc;}");
        btn->setIcon(pixmap);
        btn->setIconSize(QSize(60, 60));
        btn->setFont(QFont("Microsoft YaHei UI", 13));
        btn->setCheckable(true);
        vlayout->addWidget(btn);

        // insert widget
        PetInfo *petinfowidget = new PetInfo(this->httpclass, datamap[i]["pet_id"].toInt(), ui->petScrollAreaContent);
        petinfowidget->setFixedWidth(411);
        petinfowidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
        petinfowidget->setVisible(false);
        vlayout->addWidget(petinfowidget);

        connect(btn, &QPushButton::clicked, this, [=](){
            if(btn->isChecked() == true)
            {
                petinfowidget->setVisible(true);
                petinfowidget->getonepetRequest();
            }
            else
            {
                petinfowidget->setVisible(false);
            }
        }, Qt::AutoConnection);
        connect(petinfowidget, &PetInfo::deleteBtnclicked, this, [=](){
            petinfowidget->hide();
            btn->hide();
            delete btn;
            delete petinfowidget;
        }, Qt::AutoConnection);
    }

    QPushButton *newbtn = new QPushButton;
    newbtn->setParent(ui->petScrollAreaContent);
    newbtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    QPixmap pixmap;
    pixmap.load(":/res/publish_icon.png");
//    newbtn->setFixedWidth(411);
    newbtn->setFixedSize(411, 100);
    newbtn->setStyleSheet("QPushButton{border:1px solid #5ca4a9;background:#9bc1bc;}");
    newbtn->setIcon(pixmap);
    newbtn->setIconSize(QSize(60, 60));
    newbtn->setFont(QFont("Microsoft YaHei UI", 13));
    newbtn->setCheckable(true);
    vlayout->addWidget(newbtn);

    // insert widget
    PetInfo *newpetinfowidget = new PetInfo(this->httpclass, -1, ui->petScrollAreaContent);
    newpetinfowidget->setFixedWidth(411);
    newpetinfowidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    newpetinfowidget->setVisible(false);
    vlayout->addWidget(newpetinfowidget);

    connect(newbtn, &QPushButton::clicked, this, [=](){
        if(newbtn->isChecked() == true)
        {
            newpetinfowidget->setVisible(true);
            newpetinfowidget->createPet();
        }
        else
        {
            newpetinfowidget->setVisible(false);
        }
    }, Qt::AutoConnection);
    connect(newpetinfowidget, &PetInfo::deleteBtnclicked, this, [=](){
        newpetinfowidget->setVisible(false);
        newbtn->setChecked(false);
    }, Qt::AutoConnection);
    connect(newpetinfowidget, &PetInfo::createSuccess, this, [=](){
        this->petBtn_clicked();
    }, Qt::AutoConnection);

    // bind layout
    ui->petScrollAreaContent->setLayout(vlayout);
}

void HomePage::initProfileContent()
{
    // get user info request send to userinfo/<int:pk>/
    // ?cache
    QString strurl = "userinfo/" + QString::number(this->httpclass->m_uid) + "/";
    QString strRes = this->httpclass->getrequest(nullptr, 1, strurl);
    QJsonObject objRes = this->httpclass->parseResponse(strRes);
    QString avatarurl;
    if (!objRes.empty())
    {
        int code = objRes.take("code").toInt();
        if (code == 200)
        {
            QJsonObject dataRes = objRes.value("data").toObject();
            ui->headusernamelabel->setText("@" + dataRes.value("username").toString());
            ui->headnamelabel->setText(dataRes.value("name").toString());
            ui->headtimelabel->setText("joined " + dataRes.value("join_time").toString());
            ui->headfollowinglabel->setText(QString::number(dataRes.value("following").toInt()) + " Following");
            ui->headfollowerlabel->setText(QString::number(dataRes.value("followed").toInt()) + " Follower");
            ui->namelineedit->setText(dataRes.value("name").toString());
            ui->sexlineedit->setText(dataRes.value("gender_display").toString());
            ui->agelineedit->setText(QString::number(dataRes.value("age").toInt()));
            ui->emaillineedit->setText(dataRes.value("email").toString());
            ui->phonelineedit->setText(dataRes.value("phone").toString());
            avatarurl = dataRes.value("avatar").toString();
        }
        else if (code == 400)
        {
            QMessageBox::information(ui->profilewidget, "Error", objRes.value("msg").toString());
            return;
        }
        else
        {
            QMessageBox::information(ui->profilewidget, "Error", "Network error!");
            return;
        }
    }

    if (avatarurl != "")
    {
        QString strurl = this->httpclass->m_rootUrl + avatarurl.remove(0, 1);
        strurl.remove("/api/v1");
        QPixmap pixmap = this->httpclass->downloadphoto(strurl);
        if (!pixmap)
        {
            pixmap.load(":/res/person_icon.png");
        }
        pixmap = pixmap.scaled(ui->avatarBtn->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        QBitmap mask(pixmap.size());
        QPainter painter(&mask);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setRenderHint(QPainter::SmoothPixmapTransform);
        painter.fillRect(0, 0, pixmap.width(), pixmap.height(), Qt::white);
        painter.setBrush(QColor(0, 0, 0));
        painter.drawRoundedRect(0, 0, pixmap.width(), pixmap.height(), 99, 99);
        pixmap.setMask(mask);
        ui->avatarBtn->setIcon(pixmap);
    }

    ui->namelineedit->setReadOnly(true);
    ui->sexlineedit->setReadOnly(true);
    ui->agelineedit->setReadOnly(true);
    ui->emaillineedit->setReadOnly(true);
    ui->phonelineedit->setReadOnly(true);

}

void HomePage::homeBtn_clicked()
{
    //    qDebug() << "clicked";
    for (int i = 0; i < this->footbtns.size(); ++i)
    {
        if (i != 0)
        {
            this->footbtns.at(i)->setChecked(false);
        }
        else
        {
            this->footbtns.at(i)->setChecked(true);
        }
    }
    footbtnCheck();
    ui->stackedWidget->setCurrentIndex(0);
}


void HomePage::followBtn_clicked()
{
    //    qDebug() << "clicked";
    for (int i = 0; i < this->footbtns.size(); ++i)
    {
        if (i != 1)
        {
            this->footbtns.at(i)->setChecked(false);
        }
        else
        {
            this->footbtns.at(i)->setChecked(true);
        }
    }
    // init followpage scrollarea
    if (ui->followScrollArea->findChild<QVBoxLayout*>() == nullptr)
    {
        initFollowSctollarea();
    }
    footbtnCheck();
    ui->stackedWidget->setCurrentIndex(1);
}

void HomePage::publishBtn_clicked()
{
//    qDebug() << "clicked";
    ui->publishBtn->setStyleSheet("QPushButton{background:#b84b3e;}");
    QTimer::singleShot(100, this, [=](){
        ui->publishBtn->setStyleSheet("QPushButton{border:0px;background:#ed6a5a;}");
    });
    QTimer::singleShot(150, this, [=](){
        if (publishBtn_flag)
        {
//            qDebug() << "show";
            this->publishBtn_flag = 0;
            this->publishPage->publishPage_Show();
        }
        else
        {
//            qDebug() << "hide";
            this->publishBtn_flag = 1;
            this->publishPage->publishPage_Hide();
        }
    });
    connect(this->publishPage, &Publish::publishBtnclicked, this, [=](){
        this->publishBtn_flag = 1;
        this->hide();
    }, Qt::AutoConnection);
    connect(this->publishPage, &Publish::publishBtnclose, this, [=](){
        this->show();
    }, Qt::AutoConnection);
}


void HomePage::petBtn_clicked()
{
    // for button click animotion
    for (int i = 0; i < this->footbtns.size(); ++i)
    {
        if (i != 3)
        {
            this->footbtns.at(i)->setChecked(false);
        }
        else
        {
            this->footbtns.at(i)->setChecked(true);
        }
    }

    // init followpage scrollarea
    QList scrollcontents = ui->petScrollAreaContent->children();

    if (!scrollcontents.isEmpty())
    {
        for (auto widget:scrollcontents)
        {
            if (widget != nullptr)
            {
                delete widget;
            }
        }
    }

    initPetSctollarea();
    footbtnCheck();
    ui->stackedWidget->setCurrentIndex(2);
}


void HomePage::mineBtn_clicked()
{
    //    qDebug() << "clicked";
    for (int i = 0; i < this->footbtns.size(); ++i)
    {
        if (i != 4)
        {
            this->footbtns.at(i)->setChecked(false);
        }
        else
        {
            this->footbtns.at(i)->setChecked(true);
        }
    }
    footbtnCheck();
    this->initProfileContent();
    ui->stackedWidget->setCurrentIndex(3);
}

void HomePage::recommendBtn_clicked()
{
    for (int i = 0; i < this->headbtns.size(); ++i)
    {
        if (i != 0)
        {
            this->headbtns.at(i)->setChecked(false);
        }
        else
        {
            this->headbtns.at(i)->setChecked(true);
        }
    }
    headbtnCheck();
    cleanbtn();
    this->resData.clear();
    this->m_resourcepage = 1;
    this->m_type = "explore";
    this->initContentSctollarea();
}


void HomePage::followheadBtn_clicked()
{
    for (int i = 0; i < this->headbtns.size(); ++i)
    {
        if (i != 1)
        {
            this->headbtns.at(i)->setChecked(false);
        }
        else
        {
            this->headbtns.at(i)->setChecked(true);
        }
    }
    headbtnCheck();
    // /api/v1/user/<int:pk>/like/
    cleanbtn();
    this->resData.clear();
    this->m_resourcepage = 1;
    this->m_type = "like";
    this->initContentSctollarea();
}

void HomePage::footbtnCheck()
{
    for (int i = 0; i < this->footbtns.size(); ++i)
    {

        if (this->footbtns.at(i)->isChecked())
        {
            this->footbtns.at(i)->setStyleSheet("QPushButton{background:#b84b3e;}");
        }
        else
        {
            this->footbtns.at(i)->setStyleSheet("QPushButton{background:#ed6a5a;}");
        }
    }
}

// foot btn check
void HomePage::headbtnCheck()
{
    for (int i = 0; i < this->headbtns.size(); ++i)
    {

        if (this->headbtns.at(i)->isChecked())
        {
            this->headbtns.at(i)->setStyleSheet("QPushButton{background:#b84b3e;}");
        }
        else
        {
            this->headbtns.at(i)->setStyleSheet("QPushButton{background:#ed6a5a;}");
        }
    }
}

void HomePage::cleanbtn()
{
    QList scrollcontents = ui->contentScrollAreaContent->children();
    if (!scrollcontents.isEmpty())
    {
        for (auto widget:scrollcontents)
        {
            if (widget != nullptr)
            {
                delete widget;
            }
        }
    }
}

void HomePage::headeditBtn_clicked()
{
    ui->bodysubmitBtn->show();
    ui->bodycancelBtn->show();
    ui->namelineedit->setReadOnly(false);
    ui->sexlineedit->setReadOnly(false);
    ui->agelineedit->setReadOnly(false);
    ui->emaillineedit->setReadOnly(false);
    ui->phonelineedit->setReadOnly(false);
}


void HomePage::bodysubmitBtn_clicked()
{
    QString sex="";
    if (ui->sexlineedit->text() == "Male")
    {
        sex = "1";
    }
    else if (ui->sexlineedit->text() == "Female")
    {
        sex = "2";
    }
    else
    {
        QMessageBox::information(this, "Error", "Please Input Male or Female");
        return;
    }

    // userinfo/<int:pk>/
    QString strurl = "userinfo/" + QString::number(this->httpclass->m_uid) + "/";
    QJsonObject requestdata;
    requestdata.insert("name", ui->namelineedit->text());
    requestdata.insert("gender", sex);
    requestdata.insert("email", ui->emaillineedit->text());
    requestdata.insert("age", ui->agelineedit->text().toInt());
    requestdata.insert("phone", ui->phonelineedit->text());

    // http request
    if (this->httpclass->m_uid != -1)
    {
        // send put request

        this->httpclass->putrequest(&requestdata, 1, strurl);
        // error tips
    }
    ui->bodysubmitBtn->hide();
    ui->bodycancelBtn->hide();
    ui->namelineedit->setReadOnly(true);
    ui->sexlineedit->setReadOnly(true);
    ui->agelineedit->setReadOnly(true);
    ui->emaillineedit->setReadOnly(true);
    ui->phonelineedit->setReadOnly(true);
}


void HomePage::footlogoutBtn_clicked()
{
    // send delete request to user/<int:pk>/logout/
    if (this->httpclass->m_uid != -1)
    {
        // user/<int:pk>/pet/<int:pid>/
        QString strurl = "user/" + QString::number(this->httpclass->m_uid) + "/logout/";
        this->httpclass->postrequest(nullptr, 1, strurl);
        this->userdb->deleteTable(this->httpclass->m_uid);
    }
    this->httpclass->m_uid = -1;
    this->httpclass->m_token = "";
    this->hide();
    redirect_LoginPage();
}


void HomePage::avatarBtn_clicked()
{
    // photo upload
#ifdef Q_OS_WIN
    this->m_filepath = QFileDialog::getOpenFileName(this, "Choose a File.");
#endif
#ifdef Q_OS_ANDROID
    FileSelect file_obj;
    this->m_filepath = file_obj.selectFile();
//        this->m_filepath = QFileDialog::getOpenFileName(this, "Choose a File.");
#endif

    if (this->m_filepath != "")
    {
        QFile uploadfile(this->m_filepath);
        uploadfile.open(QIODevice::ReadOnly);
        int file_length = uploadfile.size();
        QDataStream in(&uploadfile);
        char* m_buf = new char[file_length];
        in.readRawData(m_buf, file_length);

        uploadfile.close();
        QByteArray arr = QByteArray(m_buf, file_length);
        QString file_name = this->m_filepath.split("/").back();
        // upload/11/test.png/
        QString strurl = "upload/" + QString::number(this->httpclass->m_uid) + "/" + file_name + "/";
        this->httpclass->uploadavatarrequest(arr, this->m_filepath, strurl);
        QPixmap pixmap;
        pixmap.loadFromData(arr);
        ui->avatarBtn->setIcon(pixmap);
    }
}


void HomePage::bodycancelBtn_clicked()
{
    ui->bodysubmitBtn->hide();
    ui->bodycancelBtn->hide();
    ui->namelineedit->setReadOnly(true);
    ui->sexlineedit->setReadOnly(true);
    ui->agelineedit->setReadOnly(true);
    ui->emaillineedit->setReadOnly(true);
    ui->phonelineedit->setReadOnly(true);
    this->initProfileContent();
}


void HomePage::footresetBtn_clicked()
{
    this->resetpwdPage->setGeometry(this->geometry());
    this->resetpwdPage->initresetpwdPage();
    this->hide();
    this->resetpwdPage->show();
}


void HomePage::footuserresBtn_clicked()
{
    this->hide();
    this->userresPage->setGeometry(this->geometry());
    this->userresPage->initresPage();
    this->userresPage->show();
}


void HomePage::footusercommentBtn_clicked()
{
    this->hide();
    this->userresPage->setGeometry(this->geometry());
    this->userresPage->initcommentPage();
    this->userresPage->show();
}

