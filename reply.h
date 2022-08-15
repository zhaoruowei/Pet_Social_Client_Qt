#ifndef REPLY_H
#define REPLY_H

#include <QWidget>
#include <QTimer>
#include <QJsonArray>
#include "httpclass.h"

namespace Ui {
class Reply;
}

class Reply : public QWidget
{
    Q_OBJECT

public:
    explicit Reply(HttpClass *httpclass, int rid, int cid, QWidget *parent = nullptr);
    ~Reply();

    void initreplypage();

    void loadresourcedata();

    void loadreplydata();

signals:
    void redirectHomePage();

private slots:
    void backBtn_clicked();

    void clearBtn_clicked();

private:
    Ui::Reply *ui;
    // maintain httpclass
    HttpClass *httpclass;
    // maintain resource id
    int m_rid;
    // maintain reply id
    int m_cid;
    // maintain response data
    QMap<QString, QString> resData;
    // maintain reply data
    QMap<QString, QString> replyData;
};

#endif // REPLY_H
