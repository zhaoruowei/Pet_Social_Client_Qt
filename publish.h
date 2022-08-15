#ifndef PUBLISH_H
#define PUBLISH_H

#include <QWidget>
#include <QPropertyAnimation>
#include <QTimer>
#include "httpclass.h"
#include "publishres.h"

namespace Ui {
class Publish;
}

class Publish : public QWidget
{
    Q_OBJECT

public:
    explicit Publish(HttpClass *httpclass, QWidget *parent = nullptr);
    ~Publish();

    // init page
    void initpublishPage();

    void publishPage_Show();

    void publishPage_Hide();

signals:
    void publishBtnclicked();

    void publishBtnclose();

private slots:
    void textBtn_clicked();

    void imgBtn_clicked();

    void videoBtn_clicked();

private:
    Ui::Publish *ui;
    // maintain httpclass
    HttpClass *httpclass;
    // maintain animation
    QPropertyAnimation *animation;
};

#endif // PUBLISH_H
