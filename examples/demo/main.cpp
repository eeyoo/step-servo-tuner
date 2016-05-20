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

void buffer(quint8 *buf, int data);
int level(double base, double cmp);

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    /*
    quint8 buf[4];
    buffer(buf, 10000);
    qDebug() << buf[0] << " " << buf[1] << " "
                       << buf[2] << " " << buf[3];
    */
    //qDebug() << "Hello world";
    //qDebug() << "6.2 4.5 level = " << level(6.2, 4.5);
    level(6.2, 4.5);
    return app.exec();
}

void buffer(quint8 *buf, int data)
{
    buf[0] = data;
    buf[1] = data >> 8;
    buf[2] = data >> 16;
    buf[3] = data >> 24;
}

int level(double base, double cmp)
{
    qDebug() << "enter level fun...";
    //根据额定电流值判断电压参考值和电流档位

    int level = 1;
    double e = 0.0;

/*
    do {
        e = cmp - (level*base/32);
        level++;
        qDebug() << QString("level %1, e %2").arg(level).arg(e);
    } while(e>0.01);
    */

    for(int i=0; i<32; i++) {
        qDebug() << QString("level %1, e %2").arg(level).arg(e);
        e = (level*base/32) - cmp;
        if(e > 0)
            break;
        level++;
    }

    qDebug() << "end level fun...";
    return level-1; //0-31

}
