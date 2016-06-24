#ifndef COMMANDITEMLIST_H
#define COMMANDITEMLIST_H

#include <QObject>
#include <QAbstractItemModel>

#include "command.h"

class CommandItemList : public QObject
{
    Q_OBJECT
public:
    CommandItemList(QObject *parent=0);
    ~CommandItemList();

    //追加 插入 移动 修改 删除指令
    void append(Command *cmd, QStringList &list);
    void insert(Command *cmd, QStringList &list, int row);
    void move(int fromRow, int toRow);
    void edit(Command *cmd, int row);
    void del(int row);
    void clear();
    QAbstractItemModel *pmodel();
    int size();

private:
    void setRowData(int arow, QStringList &alist);

private:
    QAbstractItemModel *model;
    QList<Command*> cmd_list;
    //QStringList mStrList;
    QStringList list;
    Command *cmd;
    int row; //游标行
    int line; //选中行
    int rows; //总行数
};

#endif // COMMANDITEMLIST_H
