#include "petinfo.h"
#include "ui_petinfo.h"

PetInfo::PetInfo(HttpClass *httpclass, int pid, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PetInfo)
{
    ui->setupUi(this);
    this->httpclass = httpclass;
    this->m_pid = pid;

    connect(ui->editBtn, SIGNAL(clicked()), this, SLOT(editBtn_clicked()));
    connect(ui->delBtn, SIGNAL(clicked()), this, SLOT(delBtn_clicked()));
    connect(ui->avatarlabel, SIGNAL(clicked()), this, SLOT(avatarlabel_clicked()));

}

PetInfo::~PetInfo()
{
    delete ui;
}

void PetInfo::initPetInfoWidget()
{
    // ?cache
    QPixmap pixmap(":/res/cat_icon.png");
    if (this->m_petinfo["pet_avatar"] != "")
    {
        QString strurl = this->httpclass->m_rootUrl + this->m_petinfo["pet_avatar"].remove(0, 1);
        strurl.remove("/api/v1");
        pixmap = this->httpclass->downloadphoto(strurl);
    }
    ui->nameEdit->setReadOnly(true);
    ui->sexEdit->setReadOnly(true);
    ui->ageEdit->setReadOnly(true);
    ui->typeEdit->setReadOnly(true);
    ui->nameEdit->setText(this->m_petinfo["pet_name"]);
    ui->sexEdit->setText(this->m_petinfo["pet_gender"]);
    ui->ageEdit->setText(this->m_petinfo["pet_age"]);
    ui->typeEdit->setText(this->m_petinfo["pet_type"]);
    ui->avatarlabel->setIcon(pixmap);
    ui->avatarlabel->setFixedSize(80, 80);
}

void PetInfo::editBtn_clicked()
{
    if (this->m_edit)
    {
        this->m_edit = 0;
        ui->editBtn->setText("Submit");
        QPixmap pixmap;
        pixmap.load(":/res/submit_icon.png");
        ui->editBtn->setIcon(pixmap);
        ui->nameEdit->setReadOnly(false);
        ui->sexEdit->setReadOnly(false);
        ui->ageEdit->setReadOnly(false);
        ui->typeEdit->setReadOnly(false);
    }
    else
    {
        QString sex="";
        if (ui->sexEdit->text() == "Male")
        {
            sex = "1";
        }
        else if (ui->sexEdit->text() == "Female")
        {
            sex = "2";
        }
        else
        {
            QMessageBox::information(this, "Error", "Please Input Male or Female");
            return;
        }

        // user/<int:pk>/pet/<int:pid>/
        QString strurl = "user/" + QString::number(this->httpclass->m_uid) + "/pet/" + QString::number(this->m_pid) + "/";
        QJsonObject requestdata;
        requestdata.insert("pet_name", ui->nameEdit->text());
        requestdata.insert("pet_type", ui->typeEdit->text());
        requestdata.insert("pet_gender", sex);
        requestdata.insert("pet_age", ui->ageEdit->text().toInt());

        this->m_edit = 1;
        ui->editBtn->setText("Edit");
        QPixmap pixmap;
        pixmap.load(":/res/edit_icon.png");
        ui->editBtn->setIcon(pixmap);
        ui->nameEdit->setReadOnly(true);
        ui->sexEdit->setReadOnly(true);
        ui->ageEdit->setReadOnly(true);
        ui->typeEdit->setReadOnly(true);

        // http request
        if (this->m_pid != -1)
        {
            // send put request
            this->httpclass->putrequest(&requestdata, 1, strurl);
        }
        else
        {
            // create user/<int:pk>/pet/
            strurl = "user/" + QString::number(this->httpclass->m_uid) + "/pet/";
            QString strRes = this->httpclass->postrequest(&requestdata, 1, strurl);
            QJsonObject objRes = this->httpclass->parseResponse(strRes);
            if (!objRes.empty())
            {
                int code = objRes.take("code").toInt();
                if (code == 201)
                {
                    QMessageBox::information(this, "Success", "Create success");
                }
            }
            emit deleteBtnclicked();
            emit createSuccess();
        }
    }
}


void PetInfo::delBtn_clicked()
{
    if (this->m_pid != -1)
    {
        // user/<int:pk>/pet/<int:pid>/
        QString strurl = "user/" + QString::number(this->httpclass->m_uid) + "/pet/" + QString::number(this->m_pid) + "/";
        this->httpclass->deletequest(1, strurl);
    }
    ui->nameEdit->clear();
    ui->sexEdit->clear();
    ui->ageEdit->clear();
    ui->typeEdit->clear();
    emit deleteBtnclicked();
}

void PetInfo::getonepetRequest()
{
    // user/<int:pk>/pet/<int:pid>/
    QString strurl = "user/" + QString::number(this->httpclass->m_uid) + "/pet/" + QString::number(this->m_pid) + "/";
    QString strRes = this->httpclass->getrequest(nullptr, 1, strurl);
    QJsonObject objRes = this->httpclass->parseResponse(strRes);
    if (!objRes.empty())
    {
        int code = objRes.take("code").toInt();
        if (code == 200)
        {
            QJsonObject dataRes = objRes.value("data").toObject();
            this->m_petinfo["pet_name"] = dataRes.value("pet_name").toString();
            this->m_petinfo["pet_type"] = dataRes.value("pet_type").toString();
            this->m_petinfo["pet_gender"] = dataRes.value("pet_gender_display").toString();
            this->m_petinfo["pet_age"] = QString::number(dataRes.value("pet_age").toInt());
            this->m_petinfo["pet_avatar"] = dataRes.value("pet_avatar").toString();
        }
    }

    this->initPetInfoWidget();
}

void PetInfo::createPet()
{
    ui->nameEdit->setReadOnly(false);
    ui->sexEdit->setReadOnly(false);
    ui->ageEdit->setReadOnly(false);
    ui->typeEdit->setReadOnly(false);
    ui->editBtn->setText("Submit");
    QPixmap pixmap;
    pixmap.load(":/res/submit_icon.png");
    ui->editBtn->setIcon(pixmap);
}

void PetInfo::avatarlabel_clicked()
{
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
        // upload/11/pet/2/test.png/
        QString strurl = "upload/" + QString::number(this->httpclass->m_uid) + "/pet/" + QString::number(this->m_pid) + "/" + file_name + "/";
        this->httpclass->uploadavatarrequest(arr, this->m_filepath, strurl);
        QPixmap pixmap;
        pixmap.loadFromData(arr);
        ui->avatarlabel->setIcon(pixmap);
    }
}

