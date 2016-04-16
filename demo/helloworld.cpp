#include "helloworld.h"

HelloWorld::HelloWorld()
{

}

quint8* HelloWorld::convert(quint32 n)
{
    static quint8 temp[4];
    temp[0] = n;
    temp[1] = n >> 8;
    temp[2] = n >> 16;
    temp[3] = n >> 24;

    return temp;
}

QByteArray HelloWorld::raw(quint8 *p, int size)
{
    QByteArray qa;
    for(int i=0; i < size; i++) {
        qa.append(*(p+size));
    }
    return qa;
}
