#ifndef RESETPWD_H
#define RESETPWD_H

#include <QWidget>
#include <QJsonObject>
#include "httpclass.h"

namespace Ui {
class ResetPwd;
}

class ResetPwd : public QWidget
{
    Q_OBJECT

public:
    explicit ResetPwd(HttpClass *httpclass, QWidget *parent = nullptr);
    ~ResetPwd();
    void initresetpwdPage();

private slots:
    void submitBtn_clicked();

    void cancelBtn_clicked();

signals:
    void redirectHomePage();

private:
    Ui::ResetPwd *ui;
    // maintain http class
    HttpClass *httpclass;
};

#endif // RESETPWD_H
