#ifndef COMMAND_H
#define COMMAND_H

#include <QObject>

/**
 * @brief 指令基类
 */

#define ABS_MOVE_CMD 0x01 //绝对运动
#define SETMOVESPCMD 0x03 //目标速度
#define EMSTOP_CMD   0x0f //停止

#define OPERATEPARAM 0x31 //操作参数命令 1)++  2)--
#define JMP_CMD      0x32 //无条件跳转
#define CMP_CMD      0x33 //有条件跳转 1)> 2)== 3<
#define IOJUMP_CMD   0x34 //IO条件跳转 0-低电平 1-高电平 仅包括1-4号端口
#define DELAY_CMD    0x35 //延时等待单位毫秒

#define SETOUT_CMD   0x40 //设置输出IO端口值
#define INPUT_CMD    0x41 //输入IO端口等待指令

#define SETACCDURCMD 0xa1 //设置加速时间(单位ms)
#define SETDECDURCMD 0xa2 //设置减速时间(单位ms)
#define SETPOSMAXPOS 0xa3 //设置正向最大允许位置(单位mm)
#define SETNEGMAXPOS 0xa4 //设置负向最大允许位置(单位mm)
#define SETPLUSCTRLT 0xa5 //设置脉冲控制方式,0x01--外部脉冲控制; 其他:内部脉冲控制
#define SETCODELOGDI 0xa6 //设置编码器逻辑方向 1-正向 0-负向
#define SETSERBAUDRT 0xa7 //RS485波特率
#define SETCANBAUDRT 0xa8 //CAN波特率
#define SETDEVICEID  0xa9 //设备ID - CAN ID
#define SETMOTORDIRE 0xaa //电机逻辑正方向

#define SETMOTDIVCMD 0xb1 //设置电机细分 1,2,4,8,32,64,128,256
#define SETCUGEARCMD 0xb2 //设置电机电流档位 1-32
#define SETREFVOLCMD 0xb3 //设置参考电压 0 - 低 1 - 高
#define SETSMARTCURR 0xb4 //设置智能电流控制值 0 - 设置电流值1/2; 1 - 设置电流值1/4

#define BATCHCONFCMD 0xca //批量配置
#define BATCHCMDNUMB 0x0a //批量配置参数命令数目
#define CMDDATANUM   0x0a //命令字节数
#define DATANUMBER   0x04 //数据字节数

#define CMDBATCHHEAD 0xcd //运行指令打包

#define EQUIPID      0x02 //设备ID

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
