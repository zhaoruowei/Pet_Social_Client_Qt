#include "reply.h"
#include "ui_reply.h"

Reply::Reply(HttpClass *httpclass, int rid, int cid, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Reply)
{
    ui->setupUi(this);

    this->httpclass = httpclass;
    this->m_rid = rid;
    this->m_cid = cid;

    this->initreplypage();

    connect(ui->backBtn, SIGNAL(clicked()), this, SLOT(backBtn_clicked()));
    connect(ui->clearBtn, SIGNAL(clicked()), this, SLOT(clearBtn_clicked()));
}

Reply::~Reply()
{
    delete ui;
}

void Reply::backBtn_clicked()
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


void Reply::clearBtn_clicked()
{
    // send delete to user/<int:pk>/resource/<int:rid>/comment/<int:cid>/
    QString strurl = "user/" + QString::number(this->httpclass->m_uid) + "/resource/" + QString::number(this->m_rid)
            + "/comment/" + QString::number(this->m_cid) + "/";
    this->httpclass->deletequest(1, strurl);
    QMessageBox::information(this, "Success", "Delete Success!");
    this->close();
    emit redirectHomePage();
}


void Reply::initreplypage()
{
    this->loadresourcedata();
    ui->headtitleedit->setReadOnly(true);
    ui->textEdit->setReadOnly(true);
    ui->headtitleedit->setText(this->resData["title"]);
    ui->headusernamelabel->setText("@"+this->resData["username"]);
    ui->headtimelabel->setText("release time: " + this->resData["time"]);
    ui->headviewedlabel->setText(this->resData["view_count"] + " viewed");
    ui->headreplylabel->setText(this->resData["reply_count"] + " reply");
    ui->headlikelabel->setText(this->resData["like_count"] + " like");

    this->loadreplydata();
    ui->textEdit->setText(this->replyData["content"]);
}

void Reply::loadresourcedata()
{
    this->resData.clear();
    // send get request to resource/<int:rid>
    QString strurl = "resource/" + QString::number(this->m_rid) + "/";
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

void Reply::loadreplydata()
{
    this->replyData.clear();
    // send get request to user/<int:pk>/resource/<int:rid>/comment/<int:cid>/
    QString strurl = "user/" + QString::number(this->httpclass->m_uid) + "/resource/" + QString::number(this->m_rid)
            + "/comment/" + QString::number(this->m_cid) + "/";
    QString strRes = this->httpclass->getrequest(nullptr, 1, strurl);
    QJsonObject objRes = this->httpclass->parseResponse(strRes);

    if (!objRes.empty())
    {
        int code = objRes.take("code").toInt();
        if (code == 200)
        {
            QJsonObject datamap = objRes.value("data").toObject();
            this->replyData["username"] = datamap.take("publisher_name").toString();
            this->replyData["time"] = datamap.take("create_time").toString();
            this->replyData["content"] = datamap.take("content").toString();
            this->replyData["uid"] = QString::number(datamap.take("publisher").toInt());
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
