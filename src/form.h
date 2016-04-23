#ifndef FORM_H
#define FORM_H

#include <QWidget>

#define ABS_MOVE_CMD 0x01 //绝对运动
#define REL_MOVE_CMD 0x02 //相对运动
#define SETMOVESPCMD 0x03 //目标速度

#define OPERATEPARAM 0x31 //操作参数命令 1)++  2)-- 3)query
#define COMPAREPARAM 0x32 //比较参数值 1)> 2)= 3<


#define SETACCDURCMD 0xa1 //设置加速时间(单位ms)
#define SETDECDURCMD 0xa2 //设置减速时间(单位ms)
#define SETPOSMAXPOS 0xa3 //设置正向最大允许位置(单位mm)
#define SETNEGMAXPOS 0xa4 //设置负向最大允许位置(单位mm)
#define SETPLUSCTRLT 0xa5 //设置脉冲控制方式,0x01--外部脉冲控制; 其他:内部脉冲控制
#define SETCODELOGDI 0xa6 //设置编码器逻辑方向 1-正向 0-负向
#define SETMOTDIVCMD 0xb1 //设置电机细分 1,2,4,8,32,64,128,256
#define SETCUGEARCMD 0xb2 //设置电机电流档位 1-32
#define SETREFVOLCMD 0xb3 //设置参考电压 0 - 低 1 - 高
#define SETSMARTCURR 0xb4 //设置智能电流控制值 0 - 设置电流值1/2; 1 - 设置电流值1/4

#define BATCHCONFCMD 0xca //批量配置
#define BATCHCMDNUMB 0x0a //批量配置参数命令数目
#define CMDDATANUM   0x0a //命令字节数
#define DATANUMBER   0x04 //数据字节数

#define EQUIPID      0x02 //设备ID


namespace Ui {
class Form;
}


class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(QWidget *parent = 0);
    ~Form();
    void receiveData(const QByteArray &data);

public slots:
    void about();

private slots: 
    //绝对运动
    //void on_absoluteMove_clicked();

    //void on_relativeMove_clicked();

    //void on_stopMove_clicked();
    void on_absAddBtn_clicked();

protected:
    void dragEnterEvent(QDragEnterEvent *event); //拖动事件
    void dropEvent(QDropEvent *event); //放置事件

signals:
    void sendData(const QByteArray &data);

private:
    void initUI();
    void initConnect();

private:
    Ui::Form *ui;
    QByteArray echo; //串口返回数据
};

#endif // FORM_H
