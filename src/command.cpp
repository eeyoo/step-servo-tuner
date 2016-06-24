#include "command.h"

#include <QtCore/QDebug>

//quint8 array[]
//QByteArray
Command::Command(QObject *parent) :
    QObject(parent)
{

}

Command::Command(int *param, CMDTYPE type) {
    //quint8 bufID[NUMBER_ID];
    //quint8 bufData[NUMBER_DA];
    //convert(bufID, id, NUMBER_ID);
    //convert(bufData, param, NUMBER_DA);
    mType = type;
    init(type);
    quint8 buf[NUMBER_CMD];

    switch (type) {
    case ABS://绝对运行
    case RELA://相对运行
        parse(buf, param, ABS_MOVE_CMD);
        break;
    case SPD://设置速度
        parse(buf, param, SETMOVESPCMD);
        break;
    case OPER:
        parse(buf, param, OPERATEPARAM);
        break;
    case DELAY:
        parse(buf, param, DELAY_CMD);
        break;
    case JMP:
        parse(buf, param, JMP_CMD);
        break;
    case CMP:
        parse(buf, param, CMP_CMD);
        break;
    case IOJMP:
        parse(buf, param, IOJUMP_CMD);
        break;
    case SETOUT:
        parse(buf, param, SETOUT_CMD);
        break;
    case INPUT:
        parse(buf, param, INPUT_CMD);
        break;
    case STOP:
        parse(buf, param, EMSTOP_CMD);
    default:
        break;
    }

}

Command::CMDTYPE Command::type()
{
    return mType;
}

void Command::parse(quint8 *buf, int *param, int def)
{

    quint8 bufID[NUMBER_ID];
    quint8 bufData[NUMBER_DA];
    quint8 bufLow[NUMBER_LOW];
    quint8 bufHigh[NUMBER_HIGH];

    convert(bufID, param[0], NUMBER_ID); //param[0] = id

    switch (def) {
    case ABS_MOVE_CMD:
    case SETMOVESPCMD:
    case DELAY_CMD:
    case EMSTOP_CMD:
        convert(bufData, param[1], NUMBER_DA);
        buf[0] = bufID[0];
        buf[1] = bufID[1];
        buf[2] = def;
        buf[3] = 0x01;
        buf[4] = 0x00;
        buf[5] = bufData[0];
        buf[6] = bufData[1];
        buf[7] = bufData[2];
        buf[8] = bufData[3];
        buf[9] = 0x00;
        break;
    case OPERATEPARAM:
        convert(bufData, param[1], NUMBER_DA);
        buf[0] = bufID[0];
        buf[1] = bufID[1];
        buf[2] = def;
        buf[3] = param[2];
        buf[4] = 0x00;
        buf[5] = bufData[0];
        buf[6] = bufData[1];
        buf[7] = bufData[2];
        buf[8] = bufData[3];
        buf[9] = 0x00;
        break;
    case CMP_CMD:
        convert(bufLow, param[1], NUMBER_LOW);
        convert(bufHigh, param[2], NUMBER_HIGH);
        buf[0] = bufID[0];
        buf[1] = bufID[1];
        buf[2] = def;
        buf[3] = param[3];
        buf[4] = param[4];
        buf[5] = bufLow[0];
        buf[6] = bufLow[1];
        buf[7] = bufHigh[0];
        buf[8] = bufHigh[1];
        buf[9] = 0x00;
        break;
    case JMP_CMD:
    case IOJUMP_CMD:
    case INPUT_CMD:
    case SETOUT_CMD:
        convert(bufLow, param[1], NUMBER_LOW);
        convert(bufHigh, param[2], NUMBER_HIGH);
        buf[0] = bufID[0];
        buf[1] = bufID[1];
        buf[2] = def;
        buf[3] = param[3];
        buf[4] = 0x00;
        buf[5] = bufLow[0];
        buf[6] = bufLow[1];
        buf[7] = bufHigh[0];
        buf[8] = bufHigh[1];
        buf[9] = 0x00;
        break;
    default:
        break;
    }

    array2qa(qa, buf, NUMBER_CMD);
}

//ABS, RELA, SPD, OPER, JMP, CMP, IOJMP, DELAY, SETOUT, INPUT
void Command::init(CMDTYPE type)
{
    switch (type) {
    case ABS:
        qDebug() << "ABS =================> " << ABS;
        break;
    case RELA:
        qDebug() << "RELA=================> " << RELA;
        break;
    case SPD:
        qDebug() << "SPD =================> " << SPD;
        break;
    case OPER:
        qDebug() << "OPER=================> " << OPER;
        break;
    case JMP:
        qDebug() << "JMP =================> " << JMP;
        break;
    case CMP:
        qDebug() << "CMP =================> " << CMP;
        break;
    case IOJMP:
        qDebug() << "IOJMP================> " << IOJMP;
        break;
    case DELAY:
        qDebug() << "DELAY================> " << DELAY;
        break;
    case SETOUT:
        qDebug() << "SETOUT===============> " << SETOUT;
        break;
    case INPUT:
        qDebug() << "INPUT================> " << INPUT;
        break;
    default:
        break;
    }
}

Command::~Command()
{
}


QByteArray Command::data() const
{

    return qa;
}

void Command::convert(quint8 *buf, int data, int size)
{
    for(int i=0; i<size; i++)
        buf[i] = data >> (8*i);
}

void Command::array2qa(QByteArray &data, quint8 *buf, int size)
{
    for(int i=0; i<size; i++)
        data[i] = buf[i];
}
