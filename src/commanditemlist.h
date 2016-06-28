#ifndef COMMANDITEMLIST_H
#define COMMANDITEMLIST_H

#include <QObject>
#include <QAbstractItemModel>

#include "command.h"

class QJsonObject;
class CommandItemList : public QObject
{
    Q_OBJECT
public:
    CommandItemList(QObject *parent=0);
    ~CommandItemList();

    //追加 插入 移动 修改 删除指令
    void append(Command &cmd, QStringList &list);
    void insert(Command &cmd, QStringList &list, int r);
    void move(int fromRow, int toRow);
    void edit(Command &cmd, int r);
    void del(int r);
    void clear();
    QAbstractItemModel *pmodel();
    int size();
    QByteArray qa() const;
    void linecmd(int r);
    void output(QByteArray &qa);

    bool load(QString &name); //读取程序文件
    bool save(QString &name); //保存文件
    long pos();


private:
    void setRowData(int arow, Command &cmd, QStringList &alist);
    void read(const QJsonObject &json); //读取json
    void write(QJsonObject &json, int r) const; //写入json

private:
    QAbstractItemModel *model;
    QList<Command> cmd_list;
    QList<QStringList> lists;
    QList<long> moves; //相对位置增量
    QList<long> posList; //记录每行指令的绝对位置

    Command *cmd;
    //QStringList list;
    int row; //保存追加行
    int line; //保存选中行
    int rows; //保存总行数
    long position; //指令序列绝对位置
};

#endif // COMMANDITEMLIST_H
