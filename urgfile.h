#ifndef URGFILE_H
#define URGFILE_H

#include <QObject>

class UrgFile : public QObject
{
    Q_OBJECT
public:
    explicit UrgFile(QObject *parent = 0);
    int get_file_num(int pos);
signals:

public slots:
};

#endif // URGFILE_H
