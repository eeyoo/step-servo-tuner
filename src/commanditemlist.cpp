#include "commanditemlist.h"

#include <QStandardItemModel>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QtCore/QDebug>


CommandItemList::CommandItemList(QObject *parent) :
    QObject(parent), row(0), rows(0), line(0), position(0)
{

    model = new QStandardItemModel(0, 2, parent);
    model->setHeaderData(0, Qt::Horizontal, tr("指令类型"));
    model->setHeaderData(1, Qt::Horizontal, tr("指令内容"));

}

CommandItemList::~CommandItemList()
{
    delete model;
}

void CommandItemList::setRowData(int arow, Command &cmd, QStringList &alist)
{
    model->insertRow(arow, QModelIndex());
    model->setData(model->index(arow, 0), alist.at(0), Qt::DisplayRole);
    model->setData(model->index(arow, 1), alist.at(1), Qt::DisplayRole);
    model->setData(model->index(arow, 0), cmd.type(), Qt::UserRole);
    model->setData(model->index(arow, 1), cmd.type(), Qt::UserRole);
}

//追加指令
void CommandItemList::append(Command &cmd, QStringList &list)
{

    setRowData(row, cmd, list);
    cmd_list.append(cmd);
    lists.append(list);
    int type = cmd.type();
    if (type == Command::ABS) {
        position = cmd.position();
    }
    if (type == Command::RELA) {
        position = cmd.position();
    }
    if (type == Command::HOME) {
        position = 0;
    }
    moves.append(cmd.pos());


    //qDebug() << tr("绝对位置 -- %1 row -- %2").arg(position).arg(row+1);
    //qDebug() << tr("当前指令 ") << cmd.data().toHex();

    row++;
    rows++;

}

//删除 back to begin
void CommandItemList::del()
{
    row--;
    rows--;

    model->removeRow(row, QModelIndex());
    cmd_list.removeAt(row);
    lists.removeAt(row);
    moves.removeAt(row);
    position -= moves.at(row);

    if (row < 0 || rows < 0) {
        row = 0;
        rows = 0;
    }

    //qDebug() << tr("绝对位置 -- %1 行 -- %2").arg(position).arg(row+1);
    //qDebug() << tr("删除 %1 行 追加行 %2").arg(line+1).arg(row+1);
}

//清空
void CommandItemList::clear()
{
    model->removeRows(0, rows, QModelIndex());
    cmd_list.clear();
    lists.clear();
    moves.clear();
    line = 0;
    row = 0;
    rows = 0;
    position = 0;
}

QAbstractItemModel* CommandItemList::pmodel()
{
    return model;
}

int CommandItemList::size()
{
    return rows;
}

QByteArray CommandItemList::qa() const
{
    QByteArray qa;

    for (int i=0; i<rows; i++)
    {
        qa.append(cmd_list[i].data());
    }
    return qa;
}

void CommandItemList::output(QByteArray &qa)
{
    int index = 0;
    while (index < rows)
    {
        qa.append(cmd_list[index].data());
        index++;
    }

}

bool CommandItemList::load(QString &name)
{
    //加载json格式文件并解析为指令序列

    QFile loadFile(name);
    if(!loadFile.open(QIODevice::ReadOnly)) {
        //qDebug() << tr("打开文件失败");
        return false;
    }
    QByteArray saveData = loadFile.readAll();
    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));

    //lines.clear();
    clear();//清空指令序列

    QJsonObject json = loadDoc.object();
    QJsonArray array = json["program"].toArray();

    for(int i=0; i < array.size(); ++i) {
        QJsonObject object = array[i].toObject();
        read(object);
    }

    return true;
}

/*
程序文件json格式定义
{
title: model column 0
content: model column 1
type: command type
params: [] command params
data: command data
}
*/
bool CommandItemList::save(QString &name)
{
    QFile saveFile(name);
    if(!saveFile.open(QIODevice::WriteOnly)) {
        //qDebug() << tr("打开文件失败");
        return false;
    }
    QJsonObject json;
    QJsonArray array;

    for(int i=0; i<rows; i++)
    {
        QJsonObject object;
        write(object, i);

        array.append(object);
    }

    json["program"] = array;

    QJsonDocument saveDoc(json);
    saveFile.write(saveDoc.toJson());

    return true;
}

//读取一行
void CommandItemList::read(const QJsonObject &json)
{
    //读取json单元
    QStringList alist;
    alist << json["title"].toString()
          << json["content"].toString();
    //lists.append(alist);

    int type = json["type"].toInt();

    QJsonArray array = json["params"].toArray();

    Command acmd(array, type);
    //qDebug() << acmd.data().toHex();
    append(acmd, alist); //追加指令行
}


//写入一组json
void CommandItemList::write(QJsonObject &json, int r) const
{
    QStringList alist = lists[r];
    json["title"] = alist.at(0);
    json["content"] = alist.at(1);


    Command acmd = cmd_list[r];
    json["type"] = acmd.type();
    json["params"] = acmd.array();
    json["data"] = QString::fromLocal8Bit(acmd.data().toHex().data());
}

long CommandItemList::pos()
{
    return position;
}
