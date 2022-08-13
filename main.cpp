#include "homepage.h"

#include <QApplication>
#include <QScreen>
#include <QSplashScreen>
#include <QJsonObject>
#include "httpclass.h"
#include "userdb.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // welcome page, waiting homepage loading
    QSplashScreen *splash = new QSplashScreen;
    splash->setPixmap(QPixmap(":/res/splash.png"));
    splash->show();

    // Automatic screen size adaption
    QScreen *m_screen = a.primaryScreen();
    QSize m_screensize = m_screen->availableSize();

    // init db
    UserDB *userDB = new UserDB;
    // init http class
    HttpClass *httpclass = new HttpClass;
    httpclass->httpManager->setAutoDeleteReplies(true);
    // query local usertoken
    if (!userDB->queryobj->first())
    {
        httpclass->m_uid = -1;
        httpclass->m_token = "";
        for (int i = 0; i < 500; ++i)
        {
            splash->repaint();
        }
    }
    else
    {
        httpclass->m_uid = userDB->queryobj->value("UID").toInt();
        httpclass->m_token = userDB->queryobj->value("TOKEN").toString();
        // refresh token
        QString res = httpclass->postrequest(nullptr, 1, "user/refresh/");
        QJsonObject objRes = httpclass->parseResponse(res);
        if (objRes.empty() || objRes.take("code").toInt() != 200)
        {
            userDB->deleteTable(httpclass->m_uid);
            httpclass->m_uid = -1;
            httpclass->m_token = "";
        }
    }

    // show homepage
    HomePage w(m_screensize, httpclass, userDB);
//    w.show();

    // finish welcome

    splash->finish(&w);
    delete splash;

    return a.exec();
}
