#include "commanditemmodel.h"

CommandItemModel::CommandItemModel() :
    line(0)
{

}

/*
void CommandItemModel::insert(Command &cmd, int row, QStringList &list)
{
    //line++;
    //cmd_list.append(cmd);
    //插入指令
    //mIndex = CmdModelIndex(cmd);
    mStrList = list;
    cmd_list.insert(row, cmd);
    insertRow(row, QModelIndex()); //准备插入空间
    setData(index(row, 0), list.at(0), Qt::DisplayRole); //column 0
    setData(index(row, 1), list.at(1), Qt::DisplayRole); //column 1

    setData(index(row, 0), cmd.type(), Qt::UserRole); //自定义数据
    setData(index(row,1), cmd.type(), Qt::UserRole);
}

void CommandItemModel::edit(Command &cmd)
{

}

void CommandItemModel::move(Command &cmd)
{

}

void CommandItemModel::del(Command &cmd, int row)
{
    //删除选中指令行
    cmd_list.removeAt(row);//指令序列删除指定行
    removeRow(row, QModelIndex());

}
*/
