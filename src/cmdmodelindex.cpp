#include "cmdmodelindex.h"

CmdModelIndex::CmdModelIndex()
{

}

CmdModelIndex::CmdModelIndex(Command &cmd)
{
    mCmd = cmd;
}

Command::CMDTYPE CmdModelIndex::type()
{
    return mCmd.type();
}

Command CmdModelIndex::cmdData()
{
    return mCmd;
}

