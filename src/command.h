#ifndef COMMAND_H
#define COMMAND_H

#include <QObject>

/**
 * @brief 指令基类
 */

/*
#define ABS_MOVE_CMD_ID 0x01 //绝对运动
#define REL_MOVE_CMD_ID 0x02 //相对运动
#define SETSPPED_CMD_ID 0x03 //设置目标速度
#define OPERATEPARAM_ID 0x31 //操作参数命令 1)++  2)-- 3)query
#define COMPAREPARAM_ID 0x32 //比较参数值 1)> 2)= 3<
#define SETACCDURCMD_ID 0xa1 //设置加速时间(单位ms)
#define SETDECDURCMD_ID 0xa2 //设置减速时间(单位ms)
#define SETMAXPOSCMD_ID 0xa3 //设置最大允许位置(单位mm)
#define SETMOTDIVCMD_ID 0xb1 //设置电机细分 1,2,4,8,32,64,128,256
#define SETCUGEARCMD_ID 0xb2 //设置电机电流档位 1-32
#define SETREFVOLCMD_ID 0xb3 //设置参考电压 0 - 低 1 - 高
#define SETSMARTCURR_ID 0xb4 //设置智能电流控制值 0 - 设置电流值1/2; 1 - 设置电流值1/4
*/

typedef struct {
    quint8 id[2];      //命令ID
    quint8 master;     //命令码
    quint8 slave;      //子命令
    quint8 reserve;    //保留
    quint8 data[4];    //数据
    quint8 check;      //校验
} Cmd;


class Command : public QObject
{
    Q_OBJECT

public:
    explicit Command();
    ~Command();
    //不同构造方法
    Command(int id, quint8 master, quint8 slave, quint8 reserve, int data, quint8 check);
    Command(quint32 id, quint8 master, quint8 slave, quint8 reserve, quint32 data, quint8 check);

    void setIndex(int index); //设置指令编号
    void compact();  //指令打包
    QByteArray data() const; //指令转换为字符数据

private:
    quint8* convert(const int data, int size);
    QByteArray raw(quint8 *p, int size); //quint8[] -> QByteArray

private:
    Cmd *mCmd;
};

#endif // COMMAND_H
