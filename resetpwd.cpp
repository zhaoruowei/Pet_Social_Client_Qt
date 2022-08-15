#include "resetpwd.h"
#include "ui_resetpwd.h"

ResetPwd::ResetPwd(HttpClass *httpclass, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ResetPwd)
{
    ui->setupUi(this);

    // init http class
    this->httpclass = httpclass;

    // connect signal
    connect(ui->cancelBtn, SIGNAL(clicked()), this, SLOT(cancelBtn_clicked()));
    connect(ui->submitBtn, SIGNAL(clicked()), this, SLOT(submitBtn_clicked()));
}

ResetPwd::~ResetPwd()
{
    delete ui;
}

void ResetPwd::initresetpwdPage()
{
    // init
    ui->newpwdlabel->clear();
    ui->confirmpwdlabel->clear();
}

void ResetPwd::submitBtn_clicked()
{
    QString pwd = ui->newpwdlabel->text();
    QString confirmpwd = ui->confirmpwdlabel->text();
    if (pwd == "" or confirmpwd == "")
    {
        QMessageBox::information(this, "Error", "Password and Confirm Password can't be empty!");
        return;
    }
    if (pwd != confirmpwd)
    {
        QMessageBox::information(this, "Error", "Confirm Password is not the same as Password!");
        return;
    }

    // create json data
    QJsonObject requestdata;
    requestdata.insert("password", pwd);
    requestdata.insert("confirm_password", confirmpwd);

    // post request user/<int:pk>/reset/
    QString strurl = "user/" + QString::number(this->httpclass->m_uid) + "/reset/";
    QString strRes = this->httpclass->putrequest(&requestdata, 1, strurl);

    // parse response
    QJsonObject objRes = this->httpclass->parseResponse(strRes);
    if (!objRes.empty())
    {
        if (!objRes.value("detail").isUndefined())
        {
            QMessageBox::information(this, "Error", objRes.take("detail").toString());
            return;
        }

        int code = objRes.take("code").toInt();
        if (code != 200)
        {
            // handle error
            QMessageBox::information(this, "Error", objRes.take("msg").toString());
            return;
        }
        else
        {
            QMessageBox::information(this, "Success", objRes.take("msg").toString());
            // back to login page
            this->hide();
            emit this->redirectHomePage();
        }
    }
    else
    {
        QMessageBox::information(this, "Error", "Network Error");
        return;
    }
}

void ResetPwd::cancelBtn_clicked()
{
    this->hide();
    emit redirectHomePage();
}

