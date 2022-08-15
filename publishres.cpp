#include "publishres.h"
#include "ui_publishres.h"

PublishRes::PublishRes(HttpClass *httpclass, int type, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PublishRes)
{
    ui->setupUi(this);

    // init
    this->httpclass = httpclass;
    this->m_type = type;

    this->initpublishresPage();

    connect(ui->backBtn, SIGNAL(clicked()), this, SLOT(backBtn_clicked()));
    connect(ui->selectBtn, SIGNAL(clicked()), this, SLOT(selectBtn_clicked()));
    connect(ui->submitBtn, SIGNAL(clicked()), this, SLOT(submitBtn_clicked()));
    connect(ui->clearBtn, SIGNAL(clicked()), this, SLOT(clearBtn_clicked()));
}

PublishRes::~PublishRes()
{
    delete ui;
}

void PublishRes::initpublishresPage()
{
    if (m_type == 1)
    {
        ui->headBtn->setIcon(QPixmap(":/res/resource_icon.png"));
        ui->selectBtn->hide();
    }
    else if (m_type == 2)
    {
        ui->headBtn->setIcon(QPixmap(":/res/photo_select_icon.png"));
        ui->selectBtn->setIcon(QPixmap(":/res/photo_select_icon.png"));
    }
    else
    {
        ui->headBtn->setIcon(QPixmap(":/res/play_icon.png"));
        ui->selectBtn->setIcon(QPixmap(":/res/play_icon.png"));
    }
}

void PublishRes::backBtn_clicked()
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


void PublishRes::selectBtn_clicked()
{
    #ifdef Q_OS_WIN
        this->m_filepath = QFileDialog::getOpenFileName(this, "Choose a File.");
    #endif
    #ifdef Q_OS_ANDROID
        FileSelect file_obj;
        this->m_filepath = file_obj.selectFile();
//        this->m_filepath = QFileDialog::getOpenFileName(this, "Choose a File.");
    #endif
}


void PublishRes::submitBtn_clicked()
{
//    qDebug() << "click";
    QString title = ui->headtitleedit->text();
    if (title == "")
    {
        QMessageBox::information(this, "Error", "Title can't be empty!");
    }

    QJsonObject requestdata;
    requestdata.insert("title", title);
    requestdata.insert("r_type", this->m_type);
    requestdata.insert("content", ui->textEdit->toPlainText());

    // send to user/<int:pk>/resource/
    QString strurl = "user/" + QString::number(this->httpclass->m_uid) + "/resource/";
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
            this->m_rid = QString::number(objRes.take("rid").toInt());
            if (this->m_type != 1 && this->m_filepath != "")
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
                // upload/(?P<pk>\d+)/resource/(?P<rid>\d+)/(?P<filename>[^/]+)/
                QString strurl = "upload/" + QString::number(this->httpclass->m_uid) + "/resource/" + this->m_rid + "/" + file_name + "/";
                this->httpclass->uploadfilerequest(arr, this->m_filepath, strurl);
            }
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


void PublishRes::clearBtn_clicked()
{
    ui->textEdit->clear();
    ui->headtitleedit->clear();
    this->m_filepath = "";
}

