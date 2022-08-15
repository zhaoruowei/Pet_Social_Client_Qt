#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <QWidget>
#include <QDebug>
#include <QPainter>
#include <QScroller>
#include <QScrollBar>
#include <QLabel>
#include <QVBoxLayout>
#include <QTimer>
#include "userlogin.h"
#include "httpclass.h"
#include "userdb.h"
#include "usersignup.h"
#include "resetpwd.h"
#include "petinfo.h"
#include "userresource.h"
#include "publish.h"

QT_BEGIN_NAMESPACE
namespace Ui { class HomePage; }
QT_END_NAMESPACE

class HomePage : public QWidget
{
    Q_OBJECT

public:
    HomePage(QSize screensize, HttpClass *httpclass, UserDB *userdb, QWidget *parent = nullptr);
    ~HomePage();

    // init ui
    void initHomePage();
    // init footnav
    void initNav();
    void initFootNavBtn();
    // foot btn check
    void footbtnCheck();
    // init headnav
    void initHeadNavBtn();
    // foot btn check
    void headbtnCheck();

    // init content scrollarea
    void initContentSctollarea();
    // load homepage recommend content
    void loadrecommendcontent();
    // load homepage like content
    void loadlikecontent();
    // add homepage res btn
    void addresourcebtn();

    // init follow page
    void initFollowPage();
    // init follow page scrollarea
    void initFollowSctollarea();

    // init pet page
    void initPetPage();
    // init pet page scrollarea
    void initPetSctollarea();

    // init profile page
    void initProfilePage();
    // init profile content
    void initProfileContent();

private:
    // clean children obj
    void cleanbtn();

public slots:
    // redirectHomePage() signal slot
    void redirect_HomePage();
    // redirectSignupPage() signal slot
    void redirect_SignupPage();
    // redirectLoginPage() signal slot
    void redirect_LoginPage();

private slots:
    void homeBtn_clicked();

    void followBtn_clicked();

    void publishBtn_clicked();

    void petBtn_clicked();

    void mineBtn_clicked();

    void recommendBtn_clicked();

    void followheadBtn_clicked();

    void headeditBtn_clicked();

    void bodysubmitBtn_clicked();

    void footlogoutBtn_clicked();

    void avatarBtn_clicked();

    void bodycancelBtn_clicked();

    void footresetBtn_clicked();

    void footuserresBtn_clicked();

    void footusercommentBtn_clicked();

private:
    // maintain ui
    Ui::HomePage *ui;
    // maintain size
    QSize m_screenSize;
    // maintain footnavbtn
    QList<QPushButton*> footbtns;
    // maintain headnavbtn
    QList<QPushButton*> headbtns;
    // maintain userlogin page
    UserLogin *userloginPage;
    // maintain usersignup page
    UserSignup *usersignupPage;
    // maintain resetpwd page
    ResetPwd *resetpwdPage;
    // maintain *userresource Page
    UserResource *userresPage;
    // maintain http class
    HttpClass *httpclass;
    // maintain db
    UserDB *userdb;
    // maintain resource request page number
    int m_resourcepage = 1;
    // maintain resource data
    QList<QMap<QString, QString>> resData;
    // maintain pagebottom
    QString detail;
    // maintain homepage type
    QString m_type;
    // maintain publish page
    Publish *publishPage;
    // maintain publish page flag
    int publishBtn_flag = 1;
    // maintain resource page
    Resource *resourcePage;
    // maintain filepath
    QString m_filepath;
};
#endif // HOMEPAGE_H
