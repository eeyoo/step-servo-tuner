#include "absmovecmd.h"
#include <QtCore/QDebug>

AbsMoveCmd::AbsMoveCmd(Cmd *cmd):Command(cmd)
{
    mCmd = cmd;
}

QByteArray AbsMoveCmd::getData() const {
    QByteArray array;
    int size = sizeof(mCmd);
    array.resize(size);
    /*for (int i=0; i < size; i++) {
        array[i] = mCmd[i];
        qDebug(array[i]);
    }*/
    return array;
}

