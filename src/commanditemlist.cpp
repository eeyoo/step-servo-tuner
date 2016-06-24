#include "commanditemlist.h"

#include <QStandardItemModel>
#include <QtCore/QDebug>


CommandItemList::CommandItemList(QObject *parent) :
    QObject(parent), row(0), rows(0), line(0)
{

    model = new QStandardItemModel(0, 2, parent);
    model->setHeaderData(0, Qt::Horizontal, tr("指令类型"));
    model->setHeaderData(1, Qt::Horizontal, tr("指令内容"));
}

CommandItemList::~CommandItemList()
{
    delete cmd;

}

void CommandItemList::setRowData(int arow, QStringList &alist)
{
    model->insertRow(arow, QModelIndex());
    model->setData(model->index(arow, 0), alist.at(0), Qt::DisplayRole);
    model->setData(model->index(arow, 1), alist.at(1), Qt::DisplayRole);
    model->setData(model->index(arow, 0), cmd->type(), Qt::UserRole);
    model->setData(model->index(arow, 1), cmd->type(), Qt::UserRole);
}

//追加指令
void CommandItemList::append(Command *cmd, QStringList &list)
{
    this->cmd = cmd;
    this->list = list;
    setRowData(row, list);
    cmd_list.append(cmd);
    row++;
    rows++;
}

//插入指令
void CommandItemList::insert(Command *cmd, QStringList &list, int row)
{
    this->line = row;
    this->cmd = cmd;
    this->list = list;
    setRowData(line, list);
    cmd_list.insert(row, cmd);
    rows++;
    this->row = rows;
}

//移动
void CommandItemList::move(int fromRow, int toRow)
{
    qDebug() << tr("%1 行移动至 %2 行").arg(fromRow).arg(toRow);
    model->moveRow(model->index(fromRow, 0), fromRow, model->index(toRow, 0), toRow);
    model->moveRow(model->index(fromRow, 1), fromRow, model->index(toRow, 1), toRow);
    cmd_list.move(fromRow, toRow);
}

//修改
void CommandItemList::edit(Command *cmd, int row)
{
    this->cmd = cmd;
    this->row = row;
}

//删除
void CommandItemList::del(int row)
{
    this->line = row;
    model->removeRow(line, QModelIndex());
    cmd_list.removeAt(line);
    rows--;
    this->row = rows;
}

//清空
void CommandItemList::clear()
{
    model->removeRows(0, rows, QModelIndex());
    cmd_list.clear();
    line = 0;
    row = 0;
    rows = 0;
}

QAbstractItemModel* CommandItemList::pmodel()
{
    return model;
}

int CommandItemList::size()
{
    return rows;
}
