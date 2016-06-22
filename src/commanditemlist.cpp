#include "commanditemlist.h"

#include <QStandardItemModel>


CommandItemList::CommandItemList(QObject *parent) :
    QObject(parent)
{
    model = new QStandardItemModel(0, 2, parent);

}

//追加指令
void CommandItemList::append(Command &cmd)
{

    model->setItemData(QModelIndex(), QMap<>)
}

