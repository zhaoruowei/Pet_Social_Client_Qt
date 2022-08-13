#ifndef FILESELECT_H
#define FILESELECT_H

#include <QObject>


class FileSelect : public QObject
{
    Q_OBJECT
public:
    explicit FileSelect(QObject *parent = nullptr);
    void selectFile();

};

#endif // FILESELECT_H
