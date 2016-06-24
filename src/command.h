#ifndef COMMAND_H
#define COMMAND_H

#include <QObject>
//#include <QByteArray>
/**
 * @brief 指令基类
 */

#define ABS_MOVE_CMD 0x01 //绝对运动
#define SETMOVESPCMD 0x03 //目标速度

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

#define BATCHCONFCMD 0xca //配置下载
#define CMDBATCHHEAD 0xcd //指令下载

#define EMSTOP_CMD   0x0f //设备停止

#define NUMBER_ID    0x02
#define NUMBER_DA    0x04
#define NUMBER_CMD   0x0a
#define NUMBER_HIGH  0x02
#define NUMBER_LOW   0x02

/*
typedef struct {
    quint8 id[2];      //命令ID
    quint8 master;     //命令码
    quint8 slave;      //子命令
    quint8 reserve;    //保留
    quint8 data[4];    //数据
    quint8 check;      //校验
} Cmd;
*/


class Command : public QObject
{
    Q_OBJECT
public:
    enum CMDTYPE {
        ABS, RELA, SPD, OPER, JMP, CMP, IOJMP, DELAY, SETOUT, INPUT, STOP
    };
    explicit Command(QObject *parent=0);

    explicit Command(int *param, CMDTYPE type);
    //Command(int *param, CMDTYPE type);
    //Command(QByteArray &param, CMDTYPE type);
    ~Command();


    QByteArray data() const; //指令转换为字符数据
    Command::CMDTYPE type();

private:
    void parse(quint8 *buf, int *param, int def);
    //void parsejmp(quint8 *buf, int id, int *param, int def);
    void init(CMDTYPE type);
    void convert(quint8 *buf, int data, int size); //int -> quint8[4]
    void array2qa(QByteArray &data, quint8 *buf, int size); //quint8[4] -> QByteArray

private:
    //quint8 array[NUMBER_CMD];
    QByteArray qa;
    CMDTYPE mType;
};

#endif // COMMAND_H
