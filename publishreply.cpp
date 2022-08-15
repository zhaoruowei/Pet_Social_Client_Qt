#include "publishreply.h"
#include "ui_publishreply.h"

PublishReply::PublishReply(HttpClass *httpclass, int rid, QMap<QString, QString> resdata, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PublishReply)
{
    ui->setupUi(this);

    this->httpclass = httpclass;
    this->m_rid = rid;
    this->m_resdata = resdata;

    this->initpublishreplypage();

    connect(ui->backBtn, SIGNAL(clicked()), this, SLOT(backBtn_clicked()));
    connect(ui->submitBtn, SIGNAL(clicked()), this, SLOT(submitBtn_clicked()));
    connect(ui->clearBtn, SIGNAL(clicked()), this, SLOT(clearBtn_clicked()));
}

PublishReply::~PublishReply()
{
    delete ui;
}

void PublishReply::backBtn_clicked()
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

void PublishReply::initpublishreplypage()
{
    ui->headtitleedit->setReadOnly(true);
    ui->headtitleedit->setText(this->m_resdata["title"]);
    ui->headusernamelabel->setText("@"+this->m_resdata["username"]);
    ui->headtimelabel->setText("release time: " + this->m_resdata["time"]);
    ui->headviewedlabel->setText(this->m_resdata["view_count"] + " viewed");
    ui->headreplylabel->setText(this->m_resdata["reply_count"] + " reply");
    ui->headlikelabel->setText(this->m_resdata["like_count"] + " like");
}

void PublishReply::submitBtn_clicked()
{
    QString content = ui->textEdit->toPlainText();
    if (content == "")
    {
        QMessageBox::information(this, "Error", "Content can't be empty!");
    }

    QJsonObject requestdata;
    requestdata.insert("content", content);
    // send to user/<int:pk>/resource/<int:rid>/comment/
    QString strurl = "user/" + QString::number(this->httpclass->m_uid) + "/resource/" + QString::number(this->m_rid) + "/comment/";
    QString strResponse = this->httpclass->postrequest(&requestdata, 1, strurl);
    // parse response
    QJsonObject objRes = this->httpclass->parseResponse(strResponse);
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
            QJsonObject msg = objRes.value("msg").toObject();
            QString strkey =  msg.keys().at(0);
            // handle error
            QMessageBox::information(this, "Error", msg.value(strkey).toArray().at(0).toString());
            return;
        }
        else
        {
            QMessageBox::information(this, "Success", objRes.take("msg").toString());
            // back to login page
            this->close();
            emit redirectHomePage();
        }
    }
    else
    {
        QMessageBox::information(this, "Error", "Network Error");
        return;
    }
}


void PublishReply::clearBtn_clicked()
{
    ui->textEdit->clear();
}

