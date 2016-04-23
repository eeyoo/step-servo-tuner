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

typedef struct {
    int a;
    int b[3];
} strct;

typedef struct {
    quint8 id[2];      //命令ID
    quint8 master;     //命令码
    quint8 slave;      //子命令
    quint8 reserve;    //保留
    quint8 data[4];    //数据
    quint8 check;      //校验
} Cmd;

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    Cmd cmd = {1,2,3,4,5,6,7,8,9,10};
    quint8 *a = cmd.data;
    qDebug() << a[0] << " " << a[1] << " " << a[2] << " " << a[3];

    quint8 *p = &cmd;

    qDebug() << p[0];

    return app.exec();
}

