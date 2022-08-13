#include "httpclass.h"

HttpClass::HttpClass(QObject *parent)
    : QObject{parent}
{
    // init m_uid, m_token
    this->m_uid = -1;
    this->m_token = "";

    // init http manager
    this->httpManager = new QNetworkAccessManager(this);
}

bool HttpClass::isLogin(int uid, QString token)
{
    if (uid == -1 || token == "")
    {
        return false;
    }

    return true;
}

QString HttpClass::getrequest(QJsonObject *requestdata, int hasheader, QString u)
{
    // create request object
    QNetworkRequest request;

    QSslConfiguration sslConf = request.sslConfiguration();
    sslConf.setPeerVerifyMode(QSslSocket::VerifyNone);
    sslConf.setProtocol(QSsl::TlsV1_3);
    request.setSslConfiguration(sslConf);

    // set url
    QString url = this->m_rootUrl + u;
    request.setUrl(QUrl(url));

    // set header
    // 0 means no special header, just set content_type=application/json
    // 1 means with Authorization and uid
    // 2 means for fileupload, set content_type=*/*
    if (hasheader == 0)
    {
        request.setRawHeader("Content-Type", "application/json");
    }
    else if (hasheader == 1)
    {
        QByteArray bUid;
        bUid.setNum(this->m_uid);
        request.setRawHeader("Content-Type", "application/json");
        request.setRawHeader("uid", bUid);
        request.setRawHeader("Authorization", this->m_token.toUtf8());
    }
    else
    {
        // file upload
        request.setRawHeader("Content-Type", "*/*");
    }

    QString data = "";
    if (requestdata != nullptr)
    {
        // set data
        // jsonobject to jsondocument
        QJsonDocument jsonDoc(*requestdata);
        // jsondoc to string
        data = QString(jsonDoc.toJson());
    }

    // send request
    QNetworkReply *reply = this->httpManager->get(request);
    // connect reply finish signal
    QEventLoop eventLoop;
    connect(httpManager, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
    eventLoop.exec();
    // get response
    QByteArray responseData;
    responseData = reply->readAll();
    QString strResponse = QString::fromUtf8(responseData);

    return strResponse;
}

QString HttpClass::postrequest(QJsonObject *requestdata, int hasheader, QString u)
{
    // create request object
    QNetworkRequest request;

    QSslConfiguration sslConf = request.sslConfiguration();
    sslConf.setPeerVerifyMode(QSslSocket::VerifyNone);
    sslConf.setProtocol(QSsl::TlsV1_3);
    request.setSslConfiguration(sslConf);

    // set url
    QString url = this->m_rootUrl + u;
    request.setUrl(QUrl(url));

    // set header
    // 0 means no special header, just set content_type=application/json
    // 1 means with Authorization and uid
    // 2 means for fileupload, set content_type=*/*
    if (hasheader == 0)
    {
        request.setRawHeader("Content-Type", "application/json");
    }
    else if (hasheader == 1)
    {
        QByteArray bUid;
        bUid.setNum(this->m_uid);
        request.setRawHeader("Content-Type", "application/json");
        request.setRawHeader("uid", bUid);
        request.setRawHeader("Authorization", this->m_token.toUtf8());
    }
    else
    {
        // file upload
        request.setRawHeader("Content-Type", "*/*");
    }

    QString data = "";
    if (requestdata != nullptr)
    {
        // set data
        // jsonobject to jsondocument
        QJsonDocument jsonDoc(*requestdata);
        // jsondoc to string
        data = QString(jsonDoc.toJson());
    }

    // send request
    QNetworkReply *reply = httpManager->post(request, data.toUtf8());
    // connect reply finish signal
    QEventLoop eventLoop;
    connect(httpManager, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
    eventLoop.exec();
    // get response
    QByteArray responseData;
    responseData = reply->readAll();
    QString strResponse = QString::fromUtf8(responseData);
    return strResponse;
}

QString HttpClass::putrequest(QJsonObject *requestdata, int hasheader, QString u)
{
    // create request object
    QNetworkRequest request;

    QSslConfiguration sslConf = request.sslConfiguration();
    sslConf.setPeerVerifyMode(QSslSocket::VerifyNone);
    sslConf.setProtocol(QSsl::TlsV1_3);
    request.setSslConfiguration(sslConf);

    // set url
    QString url = this->m_rootUrl + u;
    request.setUrl(QUrl(url));

    // set header
    // 0 means no special header, just set content_type=application/json
    // 1 means with Authorization and uid
    // 2 means for fileupload, set content_type=*/*
    if (hasheader == 0)
    {
        request.setRawHeader("Content-Type", "application/json");
    }
    else if (hasheader == 1)
    {
        QByteArray bUid;
        bUid.setNum(this->m_uid);
        request.setRawHeader("Content-Type", "application/json");
        request.setRawHeader("uid", bUid);
        request.setRawHeader("Authorization", this->m_token.toUtf8());
    }
    else
    {
        // file upload
        request.setRawHeader("Content-Type", "*/*");
    }

    QString data = "";
    if (requestdata != nullptr)
    {
        // set data
        // jsonobject to jsondocument
        QJsonDocument jsonDoc(*requestdata);
        // jsondoc to string
        data = QString(jsonDoc.toJson());
    }

    // send request
    QNetworkReply *reply = httpManager->put(request, data.toUtf8());

    QEventLoop eventLoop;
    connect(httpManager, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
    eventLoop.exec();
    // get response
    QByteArray responseData;
    responseData = reply->readAll();
    QString strResponse = QString::fromUtf8(responseData);

    return strResponse;
}

void HttpClass::deletequest(int hasheader, QString u)
{
    // create request object
    QNetworkRequest request;

    QSslConfiguration sslConf = request.sslConfiguration();
    sslConf.setPeerVerifyMode(QSslSocket::VerifyNone);
    sslConf.setProtocol(QSsl::TlsV1_3);
    request.setSslConfiguration(sslConf);

    // set url
    QString url = this->m_rootUrl + u;
    request.setUrl(QUrl(url));

    // set header
    // 0 means no special header, just set content_type=application/json
    // 1 means with Authorization and uid
    // 2 means for fileupload, set content_type=*/*
    if (hasheader == 0)
    {
        request.setRawHeader("Content-Type", "application/json");
    }
    else if (hasheader == 1)
    {
        QByteArray bUid;
        bUid.setNum(this->m_uid);
        request.setRawHeader("Content-Type", "application/json");
        request.setRawHeader("uid", bUid);
        request.setRawHeader("Authorization", this->m_token.toUtf8());
    }
    else
    {
        // file upload
        request.setRawHeader("Content-Type", "*/*");
    }

    // send request
    httpManager->deleteResource(request);
}

// parse response
QJsonObject HttpClass::parseResponse(QString strResponse)
{
    QJsonObject objRes;
    if (strResponse.startsWith("{\"code") || strResponse.startsWith("{\"detail"))
    {
        QJsonParseError jsonError;
        QJsonDocument jsonDoc = QJsonDocument::fromJson(strResponse.toUtf8(), &jsonError);
        if (jsonError.error == QJsonParseError::NoError)
        {
            if (jsonDoc.isObject())
            {
                objRes = jsonDoc.object();
            }
        }
    }
    return objRes;
}

// get photo
QPixmap HttpClass::downloadphoto(QString url)
{
    QNetworkRequest request;

    request.setUrl(QUrl(url));
    // send request
    QNetworkReply *reply = this->httpManager->get(request);
    QEventLoop eventLoop;
    connect(httpManager, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
    eventLoop.exec();

    QByteArray responseData;
    responseData = reply->readAll();

    QPixmap pixmap;
    pixmap.loadFromData(responseData);
    return pixmap;
}
