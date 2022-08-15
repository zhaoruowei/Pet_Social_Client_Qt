#ifndef PETINFO_H
#define PETINFO_H

#include <QWidget>
#include <QFileDialog>
#include "httpclass.h"
#include "fileselect.h"

namespace Ui {
class PetInfo;
}

class PetInfo : public QWidget
{
    Q_OBJECT

public:
    explicit PetInfo(HttpClass *httpclass, int pid, QWidget *parent = nullptr);
    ~PetInfo();

private slots:
    void editBtn_clicked();

    void delBtn_clicked();

    void avatarlabel_clicked();

signals:
    void deleteBtnclicked();

    void createSuccess();

public:
    void initPetInfoWidget();

    void getonepetRequest();

    void createPet();

private:
    Ui::PetInfo *ui;
    // maintain httpclass
    HttpClass *httpclass;
    // maintain editbtn states
    bool m_edit = 1;
    // maintain pet id
    int m_pid;
    // maintain pet info
    QMap<QString, QString> m_petinfo;
    // maintain file path
    QString m_filepath;
};

#endif // PETINFO_H
