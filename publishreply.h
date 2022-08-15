#ifndef PUBLISHREPLY_H
#define PUBLISHREPLY_H

#include <QWidget>
#include <QTimer>
#include <QJsonArray>
#include "httpclass.h"

namespace Ui {
class PublishReply;
}

class PublishReply : public QWidget
{
    Q_OBJECT

public:
    explicit PublishReply(HttpClass *httpclass, int rid, QMap<QString, QString> resdata, QWidget *parent = nullptr);
    ~PublishReply();

    void initpublishreplypage();

signals:
    void redirectHomePage();

private slots:
    void backBtn_clicked();

    void submitBtn_clicked();

    void clearBtn_clicked();

private:
    Ui::PublishReply *ui;
    // maintain httpclass
    HttpClass *httpclass;
    // maintain resoruce id
    int m_rid;
    // maintain resource data
    QMap<QString, QString> m_resdata;
};

#endif // PUBLISHREPLY_H
