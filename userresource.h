#ifndef USERRESOURCE_H
#define USERRESOURCE_H

#include <QWidget>
#include <QTimer>
#include <QJsonArray>
#include <QScroller>
#include <QVBoxLayout>
#include <QScrollBar>
#include <QLabel>
#include "httpclass.h"
#include "resource.h"
#include "reply.h"

namespace Ui {
class UserResource;
}

class UserResource : public QWidget
{
    Q_OBJECT

public:
    explicit UserResource(HttpClass *httpclass, QWidget *parent = nullptr);
    ~UserResource();
    // init res page
    void initresPage();
    // init comment page
    void initcommentPage();
    // loading res,comment content
    void loadcontent();
    // add btn
    void addbtn();
    // init content
    void initcontent();
    // clean content
    void cleancontent();

signals:
    void redirectHomePage();

private slots:
    void backBtn_clicked();

private:
    Ui::UserResource *ui;
    // maintain httpclass
    HttpClass *httpclass;
    // maintain res reply
    QList<QMap<QString, QString>> m_content;
    // maintain res page number
    int m_pagenum = 1;
    // maintain type flag, 1 for res, 0 for comment
    bool flag = 1;
    // maintain resource page
    Resource *resource;
    // maintain reply page
    Reply *reply;
};

#endif // USERRESOURCE_H
