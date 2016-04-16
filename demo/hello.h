#ifndef HELLO_H
#define HELLO_H

#include <QObject>

class Hello : public QObject
{
    Q_OBJECT
public:
    explicit Hello(QObject *parent = 0);

signals:

public slots:
};

#endif // HELLO_H
