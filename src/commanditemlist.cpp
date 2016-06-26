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
    //this->cmd = cmd;
    //this->list = list;
    //追加指令为
    position = cmd.position();
    setRowData(row, cmd, list);
    cmd_list.append(cmd);
    int type = cmd.type();
    //位置增量列表
    if (type==Command::RELA) {
        posList.append(cmd.position());
        moves.append(cmd.pos());
    } else if (type==Command::ABS){
        posList.append(cmd.position());
        moves.append(0);

    }
    lists.append(list);

    row++;
    rows++;

    qDebug() << tr("绝对位置 -- %1").arg(position);
    //qDebug() << tr("当前指令 ") << cmd.data().toHex();

}

//插入指令
void CommandItemList::insert(Command &cmd, QStringList &list, int r)
{
    this->line = r;
    //this->cmd = cmd;
    //this->list = list;
    setRowData(line, cmd, list);
    int type = cmd.type();
    if(type==Command::RELA) {
        moves.insert(line, cmd.pos());
        long last = posList.at(line-1);
        posList.insert(line, last+cmd.pos());
    } else if (type == Command::ABS){
        moves.insert(line, 0);
        posList.insert(line, cmd.position());
    }

    cmd_list.insert(r, cmd);\
    lists.insert(r, list);
    rows++;
    this->row = rows;
    qDebug() << tr("绝对位置 -- %1").arg(position);
}

//移动
void CommandItemList::move(int fromRow, int toRow)
{
    qDebug() << tr("%1 行移动至 %2 行").arg(fromRow).arg(toRow);
    model->moveRow(model->index(fromRow, 0), fromRow, model->index(toRow, 0), toRow);
    model->moveRow(model->index(fromRow, 1), fromRow, model->index(toRow, 1), toRow);
    //cmd_list.move(fromRow, toRow);
}

//修改
void CommandItemList::edit(Command &cmd, int r)
{
    //this->cmd = cmd;
    this->line = r;
}

//删除
void CommandItemList::del(int r)
{
    this->line = r;
    Command selectcmd = cmd_list.at(line);

    int type = selectcmd.type();

    if (type == Command::RELA) {
        position -= selectcmd.pos();
    } else if (type == Command::ABS) {

    }

    model->removeRow(line, QModelIndex());
    cmd_list.removeAt(line);
    lists.removeAt(line);
    moves.removeAt(r);
    rows--;



    this->row = rows;

    qDebug() << tr("绝对位置 -- %1").arg(position);
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

void CommandItemList::linecmd(int r)
{
    this->line = r;
    //cmd = cmd_list.at(line);
    //qDebug() << tr("指令数据 ==> ") << cmd.data().toHex();
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
    json["params"] = acmd.array();//不同类型指令参数数量不同
    json["data"] = QString::fromLocal8Bit(acmd.data().toHex().data());
}

long CommandItemList::pos()
{
    return position;
}
