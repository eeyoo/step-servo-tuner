#ifndef CMDMODELINDEX_H
#define CMDMODELINDEX_H

#include <QModelIndex>
#include "command.h"


class CmdModelIndex : public QModelIndex
{
    Q_OBJECT
public:
    CmdModelIndex();
    CmdModelIndex(Command &cmd);

    //指令类型 指令数据
    Command::CMDTYPE type();
    Command cmdData();

private:
    Command mCmd;

};

#endif // CMDMODELINDEX_H
