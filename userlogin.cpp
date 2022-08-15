#include "userlogin.h"
#include "ui_userlogin.h"

UserLogin::UserLogin(HttpClass *httpclass, UserDB *userdb, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserLogin)
{
    ui->setupUi(this);

    // init http class
    this->httpclass = httpclass;

    // init db
    this->userdb = userdb;

    // connect loginbtn click event
    connect(ui->loginBtn, SIGNAL(clicked()), this, SLOT(loginBtn_Clicked()));
    // connect signbtn click event
    connect(ui->signupBtn, SIGNAL(clicked()), this, SLOT(signupBtn_Clicked()));
}

UserLogin::~UserLogin()
{
    delete ui;
}

// public slots
// login
void UserLogin::loginBtn_Clicked()
{
    // get user input
    QString username = ui->username_lineEdit->text();
    QString pwd = ui->pwd_lineEdit->text();

    // valid input
    if (username == "" || pwd == "")
    {
        QMessageBox::information(this, "Error", "username and password can't be empty!");
        return;
    }

    // create json data
    QJsonObject requestdata;
    requestdata.insert("username", username);
    requestdata.insert("password", pwd);

    // post request
    QString strResponse = this->httpclass->postrequest(&requestdata, 0, "user/login/");
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
        if (code != 200)
        {
            // handle error
            QMessageBox::information(this, "Error", objRes.take("msg").toString());
            return;
        }
        else
        {
            this->httpclass->m_uid = objRes.take("uid").toInt();
            this->httpclass->m_token = objRes.take("Authorization").toString();
            // insert uid token into db
            this->userdb->insertTable(this->httpclass->m_uid, this->httpclass->m_token);
            // back to home page
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
// signup
void UserLogin::signupBtn_Clicked()
{
    // redirect to signup page
    this->hide();
    emit this->redirectSignupPage();
}
