#include "command.h"

#include <QtCore/QDebug>

//quint8 array[]
//QByteArray

Command::Command(int id, int param, CMDTYPE type) {
    //quint8 bufID[NUMBER_ID];
    //quint8 bufData[NUMBER_DA];
    //convert(bufID, id, NUMBER_ID);
    //convert(bufData, param, NUMBER_DA);
    init(type);
    quint8 buf[NUMBER_CMD];

    switch (type) {
    case ABS://绝对运行
        parse(buf, id, param, ABS_MOVE_CMD);
        break;
    case RELA://相对运行
        parse(buf, id, param, ABS_MOVE_CMD);
        break;
    case SPD://设置速度
        parse(buf, id, param, SETMOVESPCMD);
        break;
    default:
        break;
    }

    /*
    if (type == ABS) {
        init(type);
        convert(bufID, id, NUMBER_ID);
        convert(bufData, param, NUMBER_DA);
        quint8 p[NUMBER_CMD] = {bufID[0],bufID[1],ABS_MOVE_CMD,0x01,0x00,bufData[0],bufData[1],bufData[2],bufData[3],0x00};
        array2qa(qa, p, NUMBER_CMD);
    } else if (type == RELA) {
        init(type);
        convert(bufID, id, NUMBER_ID);
        convert(bufData, param, NUMBER_DA);
        quint8 p[NUMBER_CMD] = {bufID[0],bufID[1],ABS_MOVE_CMD,0x01,0x00,bufData[0],bufData[1],bufData[2],bufData[3],0x00};
        array2qa(qa, p, NUMBER_CMD);
    } else if (type == SPD) {
        init(type);
        convert(bufID, id, NUMBER_ID);
        convert(bufData, param, NUMBER_DA);
        quint8 p[NUMBER_CMD] = {bufID[0],bufID[1],SETMOVESPCMD,0x01,0x00,bufData[0],bufData[1],bufData[2],bufData[3],0x00};
        array2qa(qa, p, NUMBER_CMD);
    }*/


}

void Command::parse(quint8 *buf, int id, int param, int def)
{

    quint8 bufID[NUMBER_ID];
    quint8 bufData[NUMBER_DA];
    convert(bufID, id, NUMBER_ID);
    convert(bufData, param, NUMBER_DA);

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
