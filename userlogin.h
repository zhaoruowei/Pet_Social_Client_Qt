#ifndef USERLOGIN_H
#define USERLOGIN_H

#include <QWidget>
#include <QPainter>
#include <QJsonObject>
#include <QMessageBox>
#include "httpclass.h"
#include "userdb.h"

namespace Ui {
class UserLogin;
}

class UserLogin : public QWidget
{
    Q_OBJECT

public:
    explicit UserLogin(HttpClass *httpclass, UserDB *userdb, QWidget *parent = nullptr);
    ~UserLogin();

public slots:
    // login
    void loginBtn_Clicked();
    // signup
    void signupBtn_Clicked();

signals:
    // redirect to home page
    void redirectHomePage();
    // redirect to signup page
    void redirectSignupPage();

private:
    Ui::UserLogin *ui;
    // maintain http class
    HttpClass *httpclass;
    // maintain db
    UserDB *userdb;
};

#endif // USERLOGIN_H
