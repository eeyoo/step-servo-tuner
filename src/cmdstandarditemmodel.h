#ifndef CMDSTANDARDITEMMODEL_H
#define CMDSTANDARDITEMMODEL_H
#include <QObject>
#include <QStandardItemModel>

#include "command.h"

//标准项模型子类化 - 指令专用
class CmdStandardItemModel : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit CmdStandardItemModel();

    //追加 插入 移动 修改 删除指令
    void append(Command &cmd);
    void insert(Command &cmd);
    void move(Command &cmd);
    void edit(Command &cmd);
    void del(Command &cmd);

private:
    QList<Command> mCmdList;
    QStringList mStrList;
    QStandardItem *mItem;
};

#endif // CMDSTANDARDITEMMODEL_H
