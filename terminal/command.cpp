#include "command.h"

#include <QtCore/QDebug>
Command::Command()
{
    m_cmd = new cmd_struct;
    m_get = new Cmmd;
}

void Command::putData(const QByteArray &data)
{
    //qDebug() << "Command receive command " + QString(data);
    emit getData(data);
    qDebug() << "Command receive command " + QString(data);
}

/**
 * @brief Command::setOpertateCmd 发送参数操作指令
 * @param id  指令ID
 * @param type  操作类型
 * @param paramId  参数编号
 * @param data  数据
 * @return
 */
cmd_struct Command::setOpertateCmd(qint32 id, OptTp type, qint8 paramId, qint8 &data)
{
    m_cmd->id = id;
    m_cmd->cmd_type = CmdTp::ParamOperate;
    m_cmd->child_cmd = ChdCmd::SendCmd;
    m_cmd->reserve = ResvCmd::Default;
    m_cmd->checkout = CheckOut::Default;
    m_cmd->low = type;
    m_cmd->mid_low = paramId;
    m_cmd->mid_high = data[0];
    m_cmd->high = data[1];

    return m_cmd;
}

/**
 * @brief Command::getOperateCmd 接收命令数据
 * @param id  命令ID
 * @param data 返回数据
 * @return
 */
cmd_struct Command::getOperateCmd(qint32 id, qint8 &data)
{
    m_get->id = id;
    m_get->data = data;
    return m_get;
}
