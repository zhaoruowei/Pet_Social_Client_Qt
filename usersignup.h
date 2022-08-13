#ifndef USERSIGNUP_H
#define USERSIGNUP_H

#include <QWidget>
#include <QDebug>
#include <QPainter>
#include <QJsonObject>
#include <QJsonArray>
#include <QMessageBox>
#include "httpclass.h"

namespace Ui {
class UserSignup;
}

class UserSignup : public QWidget
{
    Q_OBJECT

public:
    explicit UserSignup(HttpClass *httpclass, QWidget *parent = nullptr);
    ~UserSignup();

public slots:
    // login
    void loginBtn2_Clicked();
    // signup
    void signupBtn2_Clicked();

signals:
    // redirect to home page
    void redirectHomePage();
    // redirect to signup page
    void redirectLoginPage();

private:
    Ui::UserSignup *ui;
    // maintain http class
    HttpClass *httpclass;
};

#endif // USERSIGNUP_H
