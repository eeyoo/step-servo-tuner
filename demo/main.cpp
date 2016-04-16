#include <QCoreApplication>
#include "helloworld.h"

#include <QtCore/QDebug>

#include <iostream>
using namespace std;

/*
quint8* convert(quint32 n)
{
    static quint8 temp[4];
    temp[0] = n >> 24;
    temp[1] = n >> 16;
    temp[2] = n >> 8;
    temp[3] = n;

    return temp;
}

quint8* reverse(quint32 n)
{
    static quint8 temp[4];
    temp[0] = n;
    temp[1] = n >> 8;
    temp[2] = n >> 16;
    temp[3] = n >> 24;

    return temp;
}

QByteArray raw(quint8 *p, int size)
{
    QByteArray qa;
    //quint8 q = *(p+size);
    for(int i=0; i<size; i++)
        qa.append(*(p+i));
    return qa;
}
*/

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    /*
    quint32 in = 10000000;
    //quint8 *data = convert(in);
    quint8 *data = reverse(in);
    qDebug() << *data << " "
             << *(data+1) << " "
             << *(data+2) << " "
             << *(data+3);
    */
    quint8 data[4] = {0x65, 0x66, 0x67, 0x68};
    HelloWorld hello;
    //QByteArray array = raw(data, 4);
    QByteArray array1;
    array1.append(data[0]).append(data[1]).append(data[2]).append(data[3]);
    QByteArray array = hello.raw(data, 4);
    qDebug() << array;
    qDebug() << array1;

    return a.exec();
}

