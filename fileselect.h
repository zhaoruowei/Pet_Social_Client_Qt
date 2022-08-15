#ifndef FILESELECT_H
#define FILESELECT_H

#include <QObject>
#include <QFile>
#include <QImage>
#ifdef Q_OS_ANDROID
#include <QtCore/private/qandroidextras_p.h>
#endif

class FileSelect : public QObject
{
    Q_OBJECT
public:
    explicit FileSelect(QObject *parent = nullptr);
    QString selectFile();

};

#endif // FILESELECT_H
