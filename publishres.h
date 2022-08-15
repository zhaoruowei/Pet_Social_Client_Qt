#ifndef PUBLISHRES_H
#define PUBLISHRES_H

#include <QWidget>
#include <QPixmap>
#include <QTimer>
#include <QFileDialog>
#include <QJsonArray>
#include "httpclass.h"
#include "fileselect.h"

namespace Ui {
class PublishRes;
}

class PublishRes : public QWidget
{
    Q_OBJECT

public:
    explicit PublishRes(HttpClass *httpclass, int type, QWidget *parent = nullptr);
    ~PublishRes();

    // init page
    void initpublishresPage();

signals:
    void redirectHomePage();

private slots:
    void backBtn_clicked();

    void selectBtn_clicked();

    void submitBtn_clicked();

    void clearBtn_clicked();

private:
    Ui::PublishRes *ui;
    // maintain httpclass
    HttpClass *httpclass;
    // maintain type
    int m_type;
    // maintain filepath
    QString m_filepath = "";
    // maintain resource id
    QString m_rid;
};

#endif // PUBLISHRES_H
