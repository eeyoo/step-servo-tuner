#include "command.h"

#include <QtCore/QDebug>

Command::Command() {

}

Command::~Command()
{
    delete mCmd;
}

Command::Command(int id, quint8 master, quint8 slave, quint8 reserve, int data, quint8 check)
{

}

Command::Command(quint32 id, quint8 master, quint8 slave, quint8 reserve, quint32 data, quint8 check)
{

}

void Command::setIndex(int index)
{
    mCmd->reserve = index;
}

QByteArray Command::data() const //struct -> QByteArray
{
    QByteArray qa;

    return qa;
}

quint8* Command::convert(const int data, int size) //int -> quint8*
{
    quint8 temp[size];
    for(int i=0; i<size; i++)
        temp[i] = data >> (8*i);

    return temp;
}

void Command::compact()
{
    QByteArray qa;

}


