#include "line.h"

#include <QMetaEnum>
#include <QtCore/QDebug>

Line::Line()
{

}

Line::Line(CmdType type, int *params)
{
    mType = type;
    mParams = params;
}

Line::Line(QStringList &list)
{
    //QMetaEnum meta = QMetaEnum::fromType<CmdType>();
    //QString s = list[0];
    //mType = meta.keyToValue(s.toLocal8Bit().data());
    //mType = (CmdType)list[0];

    switch (mType) {
    case POS:
    case MOV:
    case SETSPD:
    case DELAY:
    case JMP:
        //qDebug() << QString("%1 %2").arg(s).arg(mParams[0]);
        mParams[0] = QString(list[1]).toInt();
        break;
    case OPER:
    case SETOUT:
    case INPUT:
        //s = "INPUT";
        //qDebug() << QString("%1 %2 %3").arg(s).arg(mParams[0]).arg(mParams[1]);
        mParams[0] = QString(list[1]).toInt();
        mParams[1] = QString(list[2]).toInt();
        break;
    case IOJMP:
        //s = "IOJMP";
        //qDebug() << QString("%1 %2 %3 %4").arg(s).arg(mParams[0]).arg(mParams[1]).arg(mParams[2]);
        mParams[0] = QString(list[1]).toInt();
        mParams[1] = QString(list[2]).toInt();
        mParams[2] = QString(list[3]).toInt();
        break;
    case CMP:
        //s = "CMP";
        //qDebug() << QString("%1 %2 %3 %4 %5").arg(s).arg(mParams[0]).arg(mParams[1]).arg(mParams[2]).arg(mParams[3]);
        mParams[0] = QString(list[1]).toInt();
        mParams[1] = QString(list[2]).toInt();
        mParams[2] = QString(list[3]).toInt();
        mParams[3] = QString(list[4]).toInt();
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
    case POS:
    case MOV:
    case SETSPD:
    case DELAY:
    case JMP:
        str = QString("%1 %2").arg(s).arg(mParams[0]);
        break;
    case OPER:
    case SETOUT:
    case INPUT:
        //s = "INPUT";
        str = QString("%1 %2 %3").arg(s).arg(mParams[0]).arg(mParams[1]);
        break;
    case IOJMP:
        //s = "IOJMP";
        str = QString("%1 %2 %3 %4").arg(s).arg(mParams[0]).arg(mParams[1]).arg(mParams[2]);
        break;
    case CMP:
        //s = "CMP";
        str = QString("%1 %2 %3 %4 %5").arg(s).arg(mParams[0]).arg(mParams[1]).arg(mParams[2]).arg(mParams[3]);
        break;
    default:
        break;
    }
}

//ABS, RELA, SPD, OPER, JMP, CMP, IOJMP, DELAY, SETOUT, INPUT
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
    default:
        break;
    }

    return s;
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
            list << QString("编号 %1 IO端口高电平跳转至 %2 行").arg(mParams[0]).arg(mParams[2]);
        else
            list << QString("编号 %1 IO端口低电平跳转至 %2 行").arg(mParams[0]).arg(mParams[2]);
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
