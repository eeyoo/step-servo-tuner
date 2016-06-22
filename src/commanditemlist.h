#ifndef COMMANDITEMLIST_H
#define COMMANDITEMLIST_H

#include <QObject>
#include <QAbstractItemModel>

#include "command.h"

class CommandItemList
{
    Q_OBJECT
public:
    CommandItemList(QObject *parent = 0);

    //追加 插入 移动 修改 删除指令
    void append(Command &cmd);
    void insert(Command &cmd);
    void move(Command &cmd);
    void edit(Command &cmd);
    void del(Command &cmd);

private:
    QAbstractItemModel *model;
    QList<Command> mCmdList;
    QStringList mStrList;
};

#endif // COMMANDITEMLIST_H
