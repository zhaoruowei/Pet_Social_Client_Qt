#include "usersignup.h"
#include "ui_usersignup.h"

UserSignup::UserSignup(HttpClass *httpclass, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserSignup)
{
    ui->setupUi(this);

    // init http class
    this->httpclass = httpclass;

    // connect loginbtn click event
    connect(ui->loginBtn_2, SIGNAL(clicked()), this, SLOT(loginBtn2_Clicked()));
    // connect loginbtn click event
    connect(ui->signupBtn_2, SIGNAL(clicked()), this, SLOT(signupBtn2_Clicked()));
}

UserSignup::~UserSignup()
{
    delete ui;
}
// login
void UserSignup::loginBtn2_Clicked()
{
    // redirect to signup page
    this->hide();
    emit this->redirectLoginPage();
}
// signup
void UserSignup::signupBtn2_Clicked()
{
//    qDebug() << "clicked";
    // get user input
    QString username = ui->username_lineEdit_2->text();
    QString pwd = ui->pwd_lineEdit_2->text();
    QString confirm_pwd = ui->confirmpwd_lineEdit->text();

    // valid input
    if (username == "" || pwd == "" || confirm_pwd == "")
    {
        QMessageBox::information(this, "Error", "Username, Password and Confirm Password can't be empty!");
        return;
    }
    if (pwd != confirm_pwd)
    {
        QMessageBox::information(this, "Error", "Confirm Password is not the same as Password!");
        return;
    }

    // create json data
    QJsonObject requestdata;
    requestdata.insert("username", username);
    requestdata.insert("password", pwd);
    requestdata.insert("confirm_password", confirm_pwd);

    // post request
    QString strResponse = this->httpclass->postrequest(&requestdata, 0, "user/signup/");

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
            this->hide();
            emit this->redirectLoginPage();
        }
    }
    else
    {
        QMessageBox::information(this, "Error", "Network Error");
        return;
    }
}
