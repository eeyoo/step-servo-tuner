#ifndef HELLOWORLD_H
#define HELLOWORLD_H

#include <QByteArray>

class HelloWorld
{

public:
    HelloWorld();

    quint8* convert(quint32 n);
    QByteArray raw(quint8 *p, int size);
private:

};

#endif // HELLOWORLD_H
