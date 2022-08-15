#ifndef RESOURCE_H
#define RESOURCE_H

#include <QWidget>
#include <QTimer>
#include <QScroller>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QJsonArray>
#include <QFile>
#include "httpclass.h"
#include "publishreply.h"

namespace Ui {
class Resource;
}

class Resource : public QWidget
{
    Q_OBJECT

public:
    explicit Resource(HttpClass* httpclass, int resid, int restype, QWidget *parent = nullptr);
    ~Resource();

    void initresPage();

    void initrestitleWidget();

    void loadresdata();

    void initresbodyWidget();

    void initcommentWidget();

    void loadreplydata();

    void adduploadBtn();

signals:
    void redirectHomePage();

private slots:
    void backBtn_clicked();

    void replyBtn_clicked();

    void likeBtn_clicked();

    void followBtn_clicked();

    void editBtn_clicked();

    void submitBtn_clicked();

    void deleteBtn_clicked();

private:
    Ui::Resource *ui;
    // maintain httpclass
    HttpClass* httpclass;
    // maintain res/comment id
    int m_id;
    // maintain type(1:res, 0:comment)
    int m_type;
    // maintain like flag
    int like_flag;
    // maintain follow flag
    int follow_flag;
    // maintain response data
    QMap<QString, QString> resData;
    // maintain reply data
    QList<QMap<QString, QString>> replyData;
    // maintain upload file
    QFile *uploadfile = nullptr;

};

#endif // RESOURCE_H
