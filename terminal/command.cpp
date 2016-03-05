#include "command.h"

#include <QtCore/QDebug>
Command::Command()
{

}

void Command::putData(const QByteArray &data)
{
    //qDebug() << "Command receive command " + QString(data);
    emit getData(data);
    qDebug() << "Command receive command " + QString(data);
}

