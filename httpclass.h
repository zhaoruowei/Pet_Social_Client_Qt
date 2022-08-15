#ifndef HTTPCLASS_H
#define HTTPCLASS_H

#include <QObject>
#include <QJsonObject>
#include <QJsonDocument>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QMessageBox>

class HttpClass : public QObject
{
    Q_OBJECT
public:
    explicit HttpClass(QObject *parent = nullptr);

    // check login status
    bool isLogin(int uid, QString token);

    // headerType choose from 0,1,2
    // 0 means no special header, just set content_type=application/json
    // 1 means with Authenticate and uid
    // 2 means for fileupload, set content_type=*/*
    // get http request
    QString getrequest(QJsonObject *requestdata, int hasheader=0, QString u="");
    // post http request with header
    QString postrequest(QJsonObject *requestdata, int hasheader=0, QString u="");
    // put http request
    QString putrequest(QJsonObject *requestdata, int hasheader=0, QString u="");
    // delete http request
    void deletequest(int hasheader=0, QString u="");
    // parse response
    QJsonObject parseResponse(QString strResponse);
    // upload file
    QString uploadfilerequest(QByteArray bytefile, QString filename, QString u="");
    QString uploadavatarrequest(QByteArray bytefile, QString filename, QString u);
    // get photo
    QPixmap downloadphoto(QString url);

public:
    // maintain uid
    int m_uid;
    // maintain token
    QString m_token;
    // maintain http comunication manager
    QNetworkAccessManager *httpManager;
    // maintain request json data
    QJsonObject *m_requestData;
    // maintain response json data
    QJsonObject *m_responseData;
    // maintain request url
//    const QString m_rootUrl = "http://pet-social-git-project-pet-test.apps.kube.eecs.qmul.ac.uk/api/v1/";
    const QString m_rootUrl = "https://www.petsocial.https.apps.kube.eecs.qmul.ac.uk/api/v1/";

};

#endif // HTTPCLASS_H
