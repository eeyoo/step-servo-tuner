#ifndef COMMANDITEMMODEL_H
#define COMMANDITEMMODEL_H

#include <QAbstractItemModel>
#include <QObject>
#include "command.h"
//#include "cmdmodelindex.h"

class CommandItemModel : public QAbstractItemModel
{
public:
    CommandItemModel();

    //插入 移动 修改 删除
    //void insert(Command &cmd);
    //void move(Command &cmd);
    //void edit(Command &cmd);
    //void del(Command &cmd);


private:
    QList<Command> cmd_list; //指令序列
    int line;                //指令游标
    QStringList mStrList;    //展示文本
    //CmdModelIndex mIndex;    //指令模型单元
};

#endif // COMMANDITEMMODEL_H
