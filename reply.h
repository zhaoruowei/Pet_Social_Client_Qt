#ifndef REPLY_H
#define REPLY_H

#include <QWidget>

namespace Ui {
class Reply;
}

class Reply : public QWidget
{
    Q_OBJECT

public:
    explicit Reply(QWidget *parent = nullptr);
    ~Reply();

private:
    Ui::Reply *ui;
};

#endif // REPLY_H
