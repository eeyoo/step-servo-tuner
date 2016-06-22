#include "cmdstandarditemmodel.h"

CmdStandardItemModel::CmdStandardItemModel()
{
    mItem = new QStandardItem();
}

void CmdStandardItemModel::append(Command &cmd)
{
    mCmdList.append(cmd);
    appendRow(QStandardItem());

}

void CmdStandardItemModel::insert(Command &cmd)
{

}
