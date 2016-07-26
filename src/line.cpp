#include "line.h"

#include <QtCore/QDebug>

Line::Line()
{

}

Line::Line(QList<int> params, CmdType type)
{
    mType = type;
    mParams = params;
}

Line::Line(int a, double b, int id) {
    alpha = a;
    beta = b;
    deviceId = id;
}

void Line::test()
{
    qDebug() << QString("%1 %2 %3").arg(alpha).arg(beta).arg(deviceId);
}

Line::Line(QStringList &list)
{
    //QString convert to enum type
    QString s = QString(list[0]).toUpper();
    str2key(s);
    //qDebug() << QString("type %1 -- %2").arg(s).arg(mType);
    mParams.clear();

    switch (mType) {
    case POS:
    case MOV:
    case SETSPD:
    case DELAY:
    case JMP:
        mParams << QString(list[1]).toInt();
        //qDebug() << QString("type %1 params %2").arg(s).arg(mParams[0]);
        break;
    case OPER:
    case SETOUT:
    case INPUT:
        mParams << QString(list[1]).toInt();
        mParams << QString(list[2]).toInt();
        //qDebug() << QString("type %1 params %2 %3").arg(s).arg(mParams[0]).arg(mParams[1]);
        break;
    case IOJMP:
        mParams << QString(list[1]).toInt();
        mParams << QString(list[2]).toInt();
        mParams << QString(list[3]).toInt();
        //qDebug() << QString("type %1 params %2 %3 %4").arg(s).arg(mParams[0]).arg(mParams[1]).arg(mParams[2]);
        break;
    case CMP:
        mParams << QString(list[1]).toInt();
        mParams << QString(list[2]).toInt();
        mParams << QString(list[3]).toInt();
        mParams << QString(list[4]).toInt();
        //qDebug() << QString("type %1 params %2 %3 %4 %5").arg(s).arg(mParams[0]).arg(mParams[1]).arg(mParams[2]).arg(mParams[3]);
        break;
    default:
        break;
    }

}

void Line::print()
{
    QString s;
    translate(mType, s);

    switch (mType) {
    case POS:
    case MOV:
    case SETSPD:
    case DELAY:
    case JMP:
        qDebug() << QString("%1 %2").arg(s).arg(mParams[0]);
        break;
    case OPER:
    case SETOUT:
    case INPUT:
        //s = "INPUT";
        qDebug() << QString("%1 %2 %3").arg(s).arg(mParams[0]).arg(mParams[1]);
        break;
    case IOJMP:
        //s = "IOJMP";
        qDebug() << QString("%1 %2 %3 %4").arg(s).arg(mParams[0]).arg(mParams[1]).arg(mParams[2]);
        break;
    case CMP:
        //s = "CMP";
        qDebug() << QString("%1 %2 %3 %4 %5").arg(s).arg(mParams[0]).arg(mParams[1]).arg(mParams[2]).arg(mParams[3]);
        break;
    default:
        break;
    }
    //qDebug() << QString("[%1 %2]").arg(s).arg(mParams[0]);
}

void Line::print(QString &str)
{
    QString s;
    translate(mType, s);

    switch (mType) {
    case HOME:
        str = QString("%1").arg(s);
        //qDebug() << str;
        break;
    case POS:
    case MOV:
    case SETSPD:
    case DELAY:
    case JMP:
        str = QString("%1 %2").arg(s).arg(mParams[0]);
        //qDebug() << str;
        break;
    case OPER:
    case SETOUT:
    case INPUT:
        //s = "INPUT";
        str = QString("%1 %2 %3").arg(s).arg(mParams[0]).arg(mParams[1]);
        //qDebug() << str;
        break;
    case IOJMP:
        //s = "IOJMP";
        str = QString("%1 %2 %3 %4").arg(s).arg(mParams[0]).arg(mParams[1]).arg(mParams[2]);
        //qDebug() << str;
        break;
    case CMP:
        //s = "CMP";
        str = QString("%1 %2 %3 %4 %5").arg(s).arg(mParams[0]).arg(mParams[1]).arg(mParams[2]).arg(mParams[3]);
        //qDebug() << str;
        break;
    default:
        break;
    }
}

//POS, MOV, SETSPD, OPER, JMP, CMP, IOJMP, DELAY, SETOUT, INPUT
QString Line::translate(CmdType type, QString &s)
{
    switch (type) {
    case POS:
        s = "POS";
        break;
    case MOV:
        s = "MOV";
        break;
    case SETSPD:
        s = "SETSPD";
        break;
    case OPER:
        s = "OPER";
        break;
    case JMP:
        s = "JMP";
        break;
    case CMP:
        s = "CMP";
        break;
    case IOJMP:
        s = "IOJMP";
        break;
    case DELAY:
        s = "DELAY";
        break;
    case SETOUT:
        s = "SETOUT";
        break;
    case INPUT:
        s = "INPUT";
        break;
    case HOME:
        s = "SETHOME";
        break;
    default:
        break;
    }

    return s;
}

//POS, MOV, SETSPD, OPER, JMP, CMP, IOJMP, DELAY, SETOUT, INPUT
void Line::str2key(QString &s)
{
    if (s == "POS")
        mType = POS;
    else if (s == "MOV")
        mType = MOV;
    else if (s == "SETSPD")
        mType = SETSPD;
    else if (s == "OPER")
        mType = OPER;
    else if (s == "JMP")
        mType = JMP;
    else if (s == "CMP")
        mType = CMP;
    else if (s == "IOJMP")
        mType = IOJMP;
    else if (s == "DELAY")
        mType = DELAY;
    else if (s == "SETOUT")
        mType = SETOUT;
    else if (s == "INPUT")
        mType = INPUT;
    else if (s == "SETHOME")
        mType = HOME;
}

void Line::strlist(QStringList &list) const
{
    //qDebug() << "line strlist called and type " << mType;

    switch (mType) {
    case POS:
        list << "绝对运行指令" << QString("绝对运行距离至 %1mm").arg(mParams[0]);
        //qDebug() << list[0] << " " << list[1];
        break;
    case MOV:
        //s = "MOV";
        list << "相对运行指令" << QString("相对运行距离 %1mm").arg(mParams[0]);
        break;
    case SETSPD:
        //s = "SETSPD";
        list << "设置速度指令" << QString("线速度设置为 %1 mm/s").arg(mParams[0]);
        break;
    case DELAY:
        //s = "DELAY";
        list << "延时等待指令" << QString("延时等待 %1 毫秒").arg(mParams[0]);
        break;
    case HOME:
        list << "零点设置指令" << QString("当前位置设置为零点位置");
        break;
    case OPER:
        //s = "OPER";
        if(mParams[1] == 0)
            list << "操作参数指令" << QString("编号 %1 参数 ++").arg(mParams[0]);
        else if(mParams[1] == 1)
            list << "操作参数指令" << QString("编号 %1 参数 --").arg(mParams[0]);
        break;
    case JMP:
        //s = "JMP";
        list << "无条件跳转指令" << QString("无条件跳转至 %1 行").arg(mParams[0]);
        break;
    case CMP:
        //s = "CMP";
        list << "有条件跳转指令";
        if(mParams[1] == 0)
            list << QString("编号 %1 参数大于 %2 跳转至 %3 行").arg(mParams[0]).arg(mParams[2]).arg(mParams[3]);
        if(mParams[1] == 1)
            list << QString("编号 %1 参数等于 %2 跳转至 %3 行").arg(mParams[0]).arg(mParams[2]).arg(mParams[3]);
        if(mParams[1] == 2)
            list << QString("编号 %1 参数小于 %2 跳转至 %3 行").arg(mParams[0]).arg(mParams[2]).arg(mParams[3]);
        break;
    case IOJMP:
        //s = "IOJMP";
        list << "IO条件跳转指令";
        if(!mParams[1])
            list << QString("编号 %1 IO端口低电平跳转至 %2 行").arg(mParams[0]).arg(mParams[2]);
        else
            list << QString("编号 %1 IO端口高电平跳转至 %2 行").arg(mParams[0]).arg(mParams[2]);
        break;
    case INPUT:
        list << "输入等待指令";
        if(!mParams[1])
            list << QString("编号 %1 输入端口低电平等待").arg(mParams[0]);
        else
            list << QString("编号 %1 输入端口高电平等待").arg(mParams[0]);
        break;
    case SETOUT:
        list << "输出设置指令";
        if(!mParams[1])
            list << QString("设置编号 %1 输出端口断开").arg(mParams[0]);
        else
            list << QString("设置编号 %1 输出端口接通").arg(mParams[0]);
        break;
    default:
        break;
    }
}

CmdType Line::type()
{
    return mType;
}

void Line::convert(quint8 *buf, int data, int size)
{
    for(int i=0; i<size; i++)
        buf[i] = data >> (8*i);
}

void Line::array2qa(QByteArray &data, quint8 *buf, int size)
{
    for(int i=0; i<size; i++)
        data[i] = buf[i];
}

QByteArray Line::data() const
{
    QByteArray qa;
    quint8 buf[NUMBER_CMD];

    quint8 bufID[NUMBER_ID];
    quint8 bufData[NUMBER_DA];
    quint8 bufLow[NUMBER_LOW];
    quint8 bufHigh[NUMBER_HIGH];

    //qDebug() << QString("alpha %1, beta %2, id %3").arg(alpha).arg(beta).arg(deviceId);

    convert(bufID, deviceId, NUMBER_ID);

    switch (mType) {
    case POS:
        convert(bufData, alpha * mParams[0], NUMBER_DA);
        buf[0] = bufID[0];
        buf[1] = bufID[1];
        buf[2] = ABS_MOVE_CMD;
        buf[3] = 0x01;
        buf[4] = 0x00;
        buf[5] = bufData[0];
        buf[6] = bufData[1];
        buf[7] = bufData[2];
        buf[8] = bufData[3];
        buf[9] = 0x00;
        break;
    case MOV:
        convert(bufData, alpha * mParams[0], NUMBER_DA);
        buf[0] = bufID[0];
        buf[1] = bufID[1];
        buf[2] = REL_MOVE_CMD;
        buf[3] = 0x01;
        buf[4] = 0x00;
        buf[5] = bufData[0];
        buf[6] = bufData[1];
        buf[7] = bufData[2];
        buf[8] = bufData[3];
        buf[9] = 0x00;
        break;
    case SETSPD:
        convert(bufData, beta * mParams[0], NUMBER_DA);
        buf[0] = bufID[0];
        buf[1] = bufID[1];
        buf[2] = SETMOVESPCMD;
        buf[3] = 0x01;
        buf[4] = 0x00;
        buf[5] = bufData[0];
        buf[6] = bufData[1];
        buf[7] = bufData[2];
        buf[8] = bufData[3];
        buf[9] = 0x00;
        break;
    case DELAY:
        convert(bufData, mParams[0], NUMBER_DA);
        buf[0] = bufID[0];
        buf[1] = bufID[1];
        buf[2] = DELAY_CMD;
        buf[3] = 0x01;
        buf[4] = 0x00;
        buf[5] = bufData[0];
        buf[6] = bufData[1];
        buf[7] = bufData[2];
        buf[8] = bufData[3];
        buf[9] = 0x00;
        break;
    case HOME:
        convert(bufData, mParams[0], NUMBER_DA);
        buf[0] = bufID[0];
        buf[1] = bufID[1];
        buf[2] = HOME_CMD;
        buf[3] = 0x01;
        buf[4] = 0x00;
        buf[5] = bufData[0];
        buf[6] = bufData[1];
        buf[7] = bufData[2];
        buf[8] = bufData[3];
        buf[9] = 0x00;
        break;
    case OPER:
        buf[0] = bufID[0];
        buf[1] = bufID[1];
        buf[2] = OPERATEPARAM;
        buf[3] = mParams[0];
        buf[4] = 0x00;
        buf[5] = mParams[1]+1;
        buf[6] = 0x00;
        buf[7] = 0x00;
        buf[8] = 0x00;
        buf[9] = 0x00;
        //qDebug() << QString("OPER: p0 %1, p1 %2").arg(mParams[0]).arg(mParams[1]);
        break;
    case JMP:
        convert(bufHigh, mParams[0]-1, NUMBER_HIGH); //下位机0行开始
        buf[0] = bufID[0];
        buf[1] = bufID[1];
        buf[2] = JMP_CMD;
        buf[3] = 0x00;
        buf[4] = 0x00;
        buf[5] = 0x00;
        buf[6] = 0x00;
        buf[7] = bufHigh[0];
        buf[8] = bufHigh[1];
        buf[9] = 0x00;
        //qDebug() << QString("JMP: p0 %1").arg(mParams[0]);
        break;
    case CMP:
        convert(bufLow, mParams[2], NUMBER_LOW);
        convert(bufHigh, mParams[3]-1, NUMBER_HIGH); //下位机0行开始
        buf[0] = bufID[0];
        buf[1] = bufID[1];
        buf[2] = CMP_CMD;
        buf[3] = mParams[0];
        buf[4] = mParams[1]+1;
        buf[5] = bufLow[0];
        buf[6] = bufLow[1];
        buf[7] = bufHigh[0];
        buf[8] = bufHigh[1];
        buf[9] = 0x00;
        //qDebug() << QString("CMP: p0 %1, p1 %2, p2 %3, p3 %4").arg(mParams[0]).arg(mParams[1]).arg(mParams[2]).arg(mParams[3]);
        break;
    case IOJMP:
        convert(bufLow, mParams[1], NUMBER_LOW);
        convert(bufHigh, mParams[2]-1, NUMBER_HIGH); //下位机0行开始
        buf[0] = bufID[0];
        buf[1] = bufID[1];
        buf[2] = IOJUMP_CMD;
        buf[3] = mParams[0];
        buf[4] = 0x00;
        buf[5] = bufLow[0];
        buf[6] = bufLow[1];
        buf[7] = bufHigh[0];
        buf[8] = bufHigh[1];
        buf[9] = 0x00;
        //qDebug() << QString("IOJMP: p0 %1, p1 %2, p2 %3").arg(mParams[0]).arg(mParams[1]).arg(mParams[2]);
        break;
    case INPUT:
        buf[0] = bufID[0];
        buf[1] = bufID[1];
        buf[2] = INPUT_CMD;
        buf[3] = mParams[0];
        buf[4] = 0x00;
        buf[5] = mParams[1];
        buf[6] = 0x00;
        buf[7] = 0x00;
        buf[8] = 0x00;
        buf[9] = 0x00;
        //qDebug() << QString("INPUT: p0 %1, p1 %2").arg(mParams[0]).arg(mParams[1]);
        break;
    case SETOUT:
        buf[0] = bufID[0];
        buf[1] = bufID[1];
        buf[2] = SETOUT_CMD;
        buf[3] = mParams[0];
        buf[4] = 0x00;
        buf[5] = mParams[1];
        buf[6] = 0x00;
        buf[7] = 0x00;
        buf[8] = 0x00;
        buf[9] = 0x00;
        //qDebug() << QString("SETOUT: p0 %1, p1 %2").arg(mParams[0]).arg(mParams[1]);
        break;
    case HEAD:
        convert(bufData, mParams[0], NUMBER_DA);
        buf[0] = bufID[0];
        buf[1] = bufID[1];
        buf[2] = CMDBATCHHEAD;
        buf[3] = 0x00;
        buf[4] = 0x00;
        buf[5] = bufData[0];
        buf[6] = bufData[1];
        buf[7] = bufData[2];
        buf[8] = bufData[3];
        buf[9] = 0x00;
        break;
    case STOP:
        convert(bufData, mParams[0], NUMBER_DA);
        buf[0] = bufID[0];
        buf[1] = bufID[1];
        buf[2] = EMSTOP_CMD;
        buf[3] = 0x00;
        buf[4] = 0x00;
        buf[5] = bufData[0];
        buf[6] = bufData[1];
        buf[7] = bufData[2];
        buf[8] = bufData[3];
        buf[9] = 0x00;
        break;
    default:
        break;
    }

    array2qa(qa, buf, NUMBER_CMD);

    return qa;
}
