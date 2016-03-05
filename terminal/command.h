#ifndef COMMAND_H
#define COMMAND_H

#include <QObject>

/**
 * @brief 指令基类
 */

struct cmd_struct {
    qint32 id;          //命令ID
    qint8 cmd_type;     //命令码
    qint8 child_cmd;    //子命令
    qint8 reserve;      //备用保留
    qint8 low;          //数据低位
    qint8 mid_low;      //数据中低
    qint8 mid_high;     //数据中高
    qint8 high;         //数据高位
    qint8 checkout;     //校验
};

struct Cmmd {
    qint32 id;
    qint8 data[8];
};

class Command : public QObject
{
    Q_OBJECT

public:
    explicit Command();

    void putData(const QByteArray &data);



    enum CmdTp {                //命令码
        AbsMoveCmd = 0x01,      //绝对运动
        RelMoveCmd = 0x02,      //相对运动
        SetGoalSpd = 0x03,      //设置目标速度
        ParamOperate = 0x31,    //操作参数命令 1)++  2)-- 3)query
        ParamJudge = 0x32,      //比较参数值 1)> 2)= 3<
        SetAccDura = 0xA1,      //设置加速时间(单位ms)
        SetDecDura = 0xA2,      //设置减速时间(单位ms)
        SetMaxPos = 0xA3,       //设置最大允许位置(单位mm)
        SetMotorDiv = 0xB1,     //设置电机细分 1,2,4,8,32,64,128,256
        SetCurGear = 0xB2,      //设置电机电流档位 1-32
        SetVolLevel = 0xB3,     //设置参考电压 0 - 低 1 - 高
        SetCurSmtCtrl = 0xB4   //设置智能电流控制值 0 - 设置电流值1/2; 1 - 设置电流值1/4
    };
    Q_ENUM(CmdTp)

    /*
    enum ParamID {              //参数编号 1-20
        P1 = 0x01,P2,P3,P4,P5,P6,P7,P8,P9,P10,P11,P12,P13,P14,P15,P16,P17,P18,P19,P20
    };
    Q_ENUM(ParamID)
    */

    enum ChdCmd {              //子命令
        SendCmd = 0x01,         //上位机发送命令
        RecvCmd = 0x02        //下位机返回命令
    };
    Q_ENUM(ChdCmd)

    enum ResvCmd {              //备用
        Default = 0x00,        //默认为0
        GreaterThan = 0x01,     //判断类型为大于
        EqualTo = 0x02,          //等于
        LessThan = 0x03         //小于
    };
    Q_ENUM(ResvCmd)

    enum CheckOut {
        Default = 0x00         //默认为0
    };
    Q_ENUM(CheckOut)

    enum OptTp {
        Ind = 0x01,   //++
        Dec = 0x02,   //--
        Query = 0x03  //查询操作
    };
    Q_ENUM(OptTp)

    cmd_struct setOpertateCmd(qint32 id, OptTp type, qint8 paramId, qint8 &data);  //发送参数操作命令
    Cmmd getOperateCmd(qint32 id, qint8 &data);    //返回参数操作命令

    //cmd_struct getJudgeCmd();


signals:
    void getData(const QByteArray &data);

private:
    //Command *operate;  //参数操作命令
    //Command *judge;     //参数判断命令
    cmd_struct *m_cmd;   //命令结构体
    Cmmd *m_get;  //接收数据
};

#endif // COMMAND_H
