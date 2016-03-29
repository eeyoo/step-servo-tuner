#ifndef SERIALCOMM_H
#define SERIALCOMM_H

#include <QObject>

class SerialComm : public QObject
{
    Q_OBJECT
public:
    explicit SerialComm(QObject *parent = 0);

signals:

public slots:
};

#endif // SERIALCOMM_H
