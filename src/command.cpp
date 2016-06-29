#include "command.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QtCore/QDebug>

#include "configdialog.h"

Command::Command(int a, double b)
{
    alpha = a;
    beta = b;
}

Command::Command(int *param, CMDTYPE type) :
    mpos(0), mp(0)
{
    //quint8 bufID[NUMBER_ID];
    //quint8 bufData[NUMBER_DA];
    //convert(bufID, id, NUMBER_ID);
    //convert(bufData, param, NUMBER_DA);
    //params = param;
    mType = type;
    //init();
    quint8 buf[NUMBER_CMD];

    switch (type) {
    case ABS://绝对运行
        abs(buf, param);
        break;
    case RELA://相对运行
        rela(buf, param);
        break;
    case SPD://设置速度
        spd(buf, param);
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
    case HEAD:
        parse(buf, param, CMDBATCHHEAD);//指令头部
    default:        
        break;
    }
}

void Command::pp()
{
    qDebug() << QString("[alpha %1, beta %2]").arg(alpha).arg(beta);
}
//文件读取
Command::Command(QJsonArray &arr, int type) :
    mpos(0), mp(0)
{
    //params = param;
    mType = (CMDTYPE)type;
    quint8 buf[NUMBER_CMD];

    switch (type) {
    case ABS://绝对运行
        fabs(buf, arr);
        break;
    case RELA://相对运行
        //parse(buf, arr, ABS_MOVE_CMD);
        frela(buf, arr);
        break;
    case SPD://设置速度
        //parse(buf, arr, SETMOVESPCMD);
        fspd(buf, arr);
        break;
    case OPER:
        parse(buf, arr, OPERATEPARAM);
        break;
    case DELAY:
        parse(buf, arr, DELAY_CMD);
        break;
    case JMP:
        parse(buf, arr, JMP_CMD);
        break;
    case CMP:
        parse(buf, arr, CMP_CMD);
        break;
    case IOJMP:
        parse(buf, arr, IOJUMP_CMD);
        break;
    case SETOUT:
        parse(buf, arr, SETOUT_CMD);
        break;
    case INPUT:
        parse(buf, arr, INPUT_CMD);
        break;
    default:
        break;
    }
}

Command::CMDTYPE Command::type()
{
    return mType;
}

void Command::abs(quint8 *buf, int *param)
{
    quint8 bufID[NUMBER_ID];
    quint8 bufData[NUMBER_DA];

    convert(bufID, param[0], NUMBER_ID); //param[0] = id
    ps.append(param[0]);
    ps.append(param[1]);
    mpos = param[1];

    convert(bufData, mpos*alpha, NUMBER_DA);
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

    array2qa(qa, buf, NUMBER_CMD);
}

void Command::rela(quint8 *buf, int *param)
{
    quint8 bufID[NUMBER_ID];
    quint8 bufData[NUMBER_DA];

    convert(bufID, param[0], NUMBER_ID); //param[0] = id
    ps.append(param[0]);
    ps.append(param[1]);
    ps.append(param[2]);

    mp = param[1];
    mpos = mp+param[2]; //pos + base

    convert(bufData, mpos*alpha, NUMBER_DA);
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

    array2qa(qa, buf, NUMBER_CMD);
}

void Command::spd(quint8 *buf, int *param)
{
    quint8 bufID[NUMBER_ID];
    quint8 bufData[NUMBER_DA];

    convert(bufID, param[0], NUMBER_ID); //param[0] = id
    ps.append(param[0]);
    ps.append(param[1]);

    int val = param[1] * beta;

    convert(bufData, val, NUMBER_DA);
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

    array2qa(qa, buf, NUMBER_CMD);
}

void Command::fabs(quint8 *buf, QJsonArray &arr)
{
    quint8 bufID[NUMBER_ID];
    quint8 bufData[NUMBER_DA];
    int id = arr[0].toInt();
    mpos = arr[1].toInt();

    convert(bufID, id, NUMBER_ID); //param[0] = id

    convert(bufData, mpos*alpha, NUMBER_DA);
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

    array2qa(qa, buf, NUMBER_CMD);
}

void Command::frela(quint8 *buf, QJsonArray &arr)
{
    quint8 bufID[NUMBER_ID];
    quint8 bufData[NUMBER_DA];
    int id = arr[0].toInt();

    mp = arr[1].toInt();
    mpos =  mp+arr[2].toInt();

    convert(bufID, id, NUMBER_ID); //param[0] = id
    convert(bufData, mpos*alpha, NUMBER_DA);

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

    array2qa(qa, buf, NUMBER_CMD);
}

void Command::fspd(quint8 *buf, QJsonArray &arr)
{
    quint8 bufID[NUMBER_ID];
    quint8 bufData[NUMBER_DA];
    int id = arr[0].toInt();
    int lsp = arr[1].toInt();

    convert(bufID, id, NUMBER_ID); //param[0] = id
    convert(bufData, lsp*beta, NUMBER_DA);

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

    array2qa(qa, buf, NUMBER_CMD);
}

void Command::parse(quint8 *buf, int *param, int def)
{

    quint8 bufID[NUMBER_ID];
    quint8 bufData[NUMBER_DA];
    quint8 bufLow[NUMBER_LOW];
    quint8 bufHigh[NUMBER_HIGH];

    convert(bufID, param[0], NUMBER_ID); //param[0] = id
    ps.append(param[0]);

    switch (def) {
    case DELAY_CMD: //2
    case EMSTOP_CMD:
    case CMDBATCHHEAD:
        ps.append(param[1]);
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
    case OPERATEPARAM: //3
        ps.append(param[1]);
        ps.append(param[2]);
        convert(bufData, param[1]+1, NUMBER_DA);
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
    case INPUT_CMD: //4
    case SETOUT_CMD:
        ps.append(param[1]);
        ps.append(param[2]);
        ps.append(param[3]);
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
    case JMP_CMD:  //4
    case IOJUMP_CMD:
        ps.append(param[1]);
        ps.append(param[2]);
        ps.append(param[3]);
        convert(bufLow, param[1], NUMBER_LOW);
        convert(bufHigh, param[2]-1, NUMBER_HIGH); //下位机0行开始
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
    case CMP_CMD: //5
        ps.append(param[1]);
        ps.append(param[2]);
        ps.append(param[3]);
        ps.append(param[4]);
        convert(bufLow, param[1], NUMBER_LOW);
        convert(bufHigh, param[2]-1, NUMBER_HIGH); //下位机0行开始
        buf[0] = bufID[0];
        buf[1] = bufID[1];
        buf[2] = def;
        buf[3] = param[3];
        buf[4] = param[4]+1;
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

void Command::parse(quint8 *buf, QJsonArray &arr, int def)
{

    quint8 bufID[NUMBER_ID];
    quint8 bufData[NUMBER_DA];
    quint8 bufLow[NUMBER_LOW];
    quint8 bufHigh[NUMBER_HIGH];

    convert(bufID, arr[0].toInt(), NUMBER_ID); //param[0] = id

    switch (def) {
    case DELAY_CMD:
        convert(bufData, arr[1].toInt(), NUMBER_DA);
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
    case OPERATEPARAM: //3
        convert(bufData, arr[1].toInt(), NUMBER_DA);
        buf[0] = bufID[0];
        buf[1] = bufID[1];
        buf[2] = def;
        buf[3] = arr[2].toInt();
        buf[4] = 0x00;
        buf[5] = bufData[0];
        buf[6] = bufData[1];
        buf[7] = bufData[2];
        buf[8] = bufData[3];
        buf[9] = 0x00;
        break;
    case JMP_CMD://4
    case IOJUMP_CMD:
    case INPUT_CMD:
    case SETOUT_CMD:
        convert(bufLow, arr[1].toInt(), NUMBER_LOW);
        convert(bufHigh, arr[2].toInt(), NUMBER_HIGH);
        buf[0] = bufID[0];
        buf[1] = bufID[1];
        buf[2] = def;
        buf[3] = arr[3].toInt();
        buf[4] = 0x00;
        buf[5] = bufLow[0];
        buf[6] = bufLow[1];
        buf[7] = bufHigh[0];
        buf[8] = bufHigh[1];
        buf[9] = 0x00;
        break;
    case CMP_CMD: //5
        convert(bufLow, arr[1].toInt(), NUMBER_LOW);
        convert(bufHigh, arr[2].toInt(), NUMBER_HIGH);
        buf[0] = bufID[0];
        buf[1] = bufID[1];
        buf[2] = def;
        buf[3] = arr[3].toInt();
        buf[4] = arr[4].toInt();
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
    /*
void Command::init(int alpha, double beta)
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

    config = new ConfigDialog;

    int level = config->configs().elecLevel;
    int circle = config->configs().circleLen;
    int div[] = {1,2,4,8,16,32,64,128,256};


    alpha = 200 * div[level] / circle;
    beta = 0.4 * div[level] / circle; // 200 * 100 / 50000

    //alpha = 1;
    //beta = 1.0;
    this->alpha = alpha;
    this->beta = beta;
}
*/
Command::~Command()
{
    //delete config;
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

QJsonArray Command::array() const
{

    QJsonArray arr;

    qDebug() << QString("指令类型 %1, 参数个数 %2]").arg(mType).arg(ps.size());

    for(int i=0; i < ps.size(); i++) {
        arr.append(ps[i]);
    }
    return arr;
}

long Command::position()
{
    return mpos;
}

long Command::pos()
{
    return mp;
}
