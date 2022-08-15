#include "userresource.h"
#include "ui_userresource.h"

UserResource::UserResource(HttpClass *httpclass, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserResource)
{
    ui->setupUi(this);

    // init httpclass
    this->httpclass = httpclass;

    // connect
    connect(ui->backBtn, SIGNAL(clicked()), this, SLOT(backBtn_clicked()));
}

UserResource::~UserResource()
{
    delete ui;
}

// init res page
void UserResource::initresPage()
{
    QPixmap pixmap;
    QString strpath = ":/res/resource_icon.png";
    bool ret = pixmap.load(strpath);
    if (!ret)
    {
        qDebug() << strpath << "load icon fail！";
    }
    ui->userresourceheadBtn->setStyleSheet("QPushButton{border:0px;background:#ed6a5a;}");
    ui->userresourceheadBtn->setIcon(pixmap);
    ui->userresourceheadBtn->setIconSize(QSize(60, 60));
    ui->userresourceheadBtn->setText("My Resource");
    this->flag = 1;
    this->initcontent();
}
// init comment page
void UserResource::initcommentPage()
{
    QPixmap pixmap;
    QString strpath = ":/res/comment_icon.png";
    bool ret = pixmap.load(strpath);
    if (!ret)
    {
        qDebug() << strpath << "load icon fail！";
    }
    ui->userresourceheadBtn->setStyleSheet("QPushButton{border:0px;background:#ed6a5a;}");
    ui->userresourceheadBtn->setIcon(pixmap);
    ui->userresourceheadBtn->setIconSize(QSize(60, 60));
    ui->userresourceheadBtn->setText("My Comment");
    this->flag = 0;
    this->initcontent();
}

void UserResource::backBtn_clicked()
{
    ui->backBtn->setStyleSheet("QPushButton{border:0px;background:#b84b3e;}");
    QTimer::singleShot(100, this, [=](){
        ui->backBtn->setStyleSheet("QPushButton{border:0px;background:#ed6a5a;}");
    });
    QTimer::singleShot(130, this, [=](){
        this->cleancontent();
        emit redirectHomePage();
    });
}

void UserResource::loadcontent()
{
    this->m_content.clear();
    QString strurl = "";
    // flag is 1 to user/<int:pk>/resource/, 0 to user/<int:pk>/comment/
    if (this->flag)
    {
        strurl = "user/" + QString::number(this->httpclass->m_uid) + "/resource/?page=" + QString::number(m_pagenum);
    }
    else
    {
        strurl = "user/" + QString::number(this->httpclass->m_uid) + "/comment/?page=" + QString::number(m_pagenum);
    }
    QString strRes = this->httpclass->getrequest(nullptr, 1, strurl);

    // parse
    QJsonObject objRes = this->httpclass->parseResponse(strRes);
    QString detail = "";
    if (!objRes.empty())
    {
        if (!objRes.value("detail").isUndefined())
        {
            detail = objRes.take("detail").toString();
            QMap<QString, QString> temp;
            temp["detail"] = detail;
            this->m_content << temp;
            return;
        }

        int code = objRes.take("code").toInt();
        if (code == 200)
        {
            QJsonArray datalist = objRes.value("data").toArray();
            for (int i = 0; i < datalist.size(); ++i)
            {
                QMap<QString, QString> tempmap;
                if (this->flag)
                {
                    tempmap["title"] = datalist[i].toObject().take("title").toString();
                    tempmap["id"] = QString::number(datalist[i].toObject().take("resource_id").toInt());
                    tempmap["release_time"] = datalist[i].toObject().take("release_time").toString();
                }
                else
                {
                    tempmap["title"] = datalist[i].toObject().take("parent_name").toString();
                    tempmap["id"] = QString::number(datalist[i].toObject().take("comment_id").toInt());
                    tempmap["release_time"] = datalist[i].toObject().take("create_time").toString();
                    tempmap["rid"] = QString::number(datalist[i].toObject().take("parent").toInt());
                }

                this->m_content << tempmap;
            }
        }
    }
}

void UserResource::initcontent()
{
    // reset page
    this->m_pagenum = 1;

    // bind grabGesture
    QScroller::grabGesture(ui->contentScrollArea->viewport(), QScroller::LeftMouseButtonGesture);

    // init pet page ui
    ui->contentScrollArea->setStyleSheet("QWidget{background:#e6ebe0;}");

    this->addbtn();

    // new page content
    connect(ui->contentScrollArea->verticalScrollBar(), &QScrollBar::valueChanged, this, [=](int val){
        if (ui->contentScrollArea->verticalScrollBar()->minimum() == val)
        {
            this->cleancontent();
            this->m_pagenum = 1;
            this->addbtn();
        }
        else if (ui->contentScrollArea->verticalScrollBar()->maximum() == val)
        {
            if (this->m_pagenum == -1)
            {
                return;
            }
            this->m_pagenum++;
            this->addbtn();
        }
    }, Qt::AutoConnection);
}

void UserResource::addbtn()
{
    // load content
    loadcontent();

    QVBoxLayout *vlayout = ui->contentScrollAreaContent->findChild<QVBoxLayout*>();
    if (vlayout == nullptr)
    {
        vlayout = new QVBoxLayout(ui->contentScrollAreaContent);
    }
    // set layout
    vlayout->setDirection(QBoxLayout::TopToBottom);
    vlayout->setSpacing(0);
    vlayout->setContentsMargins(0, 1, 0, 1);

    // show content
    // insert widget
    if (this->m_content.at(0)["detail"] != "")
    {
        this->m_pagenum = -1;
        QLabel *label = new QLabel(this);
        label->setText("No More Content.");
        label->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        vlayout->addWidget(label);
    }
    else
    {
        for (int i = 0; i < this->m_content.size(); ++i)
        {
            // insert btn
            QPushButton *btn = new QPushButton;
            btn->setParent(ui->contentScrollAreaContent);
            btn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
            btn->setText("Title: " + this->m_content.at(i)["title"]);
            btn->setObjectName(this->m_content.at(i)["id"] + "/" + this->m_content.at(i)["rid"]);
            QPixmap pixmap;
            pixmap.load(":/res/resource_icon.png");
            btn->setFixedSize(411, 100);
            btn->setStyleSheet("QPushButton{border:1px solid #5ca4a9;background:#9bc1bc;}");
            btn->setIcon(pixmap);
            btn->setIconSize(QSize(60, 60));
            btn->setFont(QFont("Microsoft YaHei UI", 13));
            vlayout->addWidget(btn);

            // btn clicked
            connect(btn, &QPushButton::clicked, this, [=](){
                btn->setStyleSheet("QPushButton{border:1px solid #5ca4a9;background:#7b9b97;}");
                QTimer::singleShot(100, this, [=](){
                    btn->setStyleSheet("QPushButton{border:1px solid #5ca4a9;background:#9bc1bc;}");
                });
                // redirect to one res
                if (this->flag)
                {
                    QTimer::singleShot(130, this, [=](){
                        int rid = btn->objectName().remove("/").toInt();
                        this->resource = new Resource(this->httpclass, rid, this->flag);
                        this->resource->show();
                        this->cleancontent();
                        this->hide();

                        connect(this->resource, &Resource::redirectHomePage, this, [=](){
                            delete this->resource;
                            this->resource = nullptr;
                            emit redirectHomePage();
                        }, Qt::AutoConnection);
                    });
                }
                else
                {
                    int cid = btn->objectName().split("/")[0].toInt();
                    int rid = btn->objectName().split("/")[1].toInt();
                    this->reply = new Reply(this->httpclass, rid, cid);
                    this->reply->show();
                    this->cleancontent();
                    this->hide();

                    connect(this->reply, &Reply::redirectHomePage, this, [=](){
                        delete this->reply;
                        this->reply = nullptr;
                        emit redirectHomePage();
                    }, Qt::AutoConnection);
                }

            }, Qt::AutoConnection);
        }
    }

    // bind layout
    ui->contentScrollAreaContent->setLayout(vlayout);
}

// clean content
void UserResource::cleancontent()
{
    // init followpage scrollarea
    QList scrollcontents = ui->contentScrollAreaContent->children();
    this->m_content.clear();
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
