#include "reply.h"
#include "ui_reply.h"

Reply::Reply(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Reply)
{
    ui->setupUi(this);
}

Reply::~Reply()
{
    delete ui;
}
