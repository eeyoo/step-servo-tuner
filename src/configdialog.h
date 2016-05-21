#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QDialog>
#include <QJsonObject>
#include <QMap>

/*
#define SETACCDURCMD 0xa1 //设置加速时间(单位ms)
#define SETDECDURCMD 0xa2 //设置减速时间(单位ms)
#define SETPOSMAXPOS 0xa3 //设置正向最大允许位置(单位mm)
#define SETNEGMAXPOS 0xa4 //设置负向最大允许位置(单位mm)
#define SETPLUSCTRLT 0xa5 //设置脉冲控制方式,0x01--外部脉冲控制; 其他:内部脉冲控制
#define SETCODELOGDI 0xa6 //设置编码器逻辑方向 1-正向 0-负向
#define SETSERBAUDRT 0xa7 //RS485波特率
#define SETCANBAUDRT 0xa8 //CAN波特率
#define SETDEVICEID  0xa9 //设备ID - CAN

#define SETMOTDIVCMD 0xb1 //设置电机细分 1,2,4,8,32,64,128,256
#define SETCUGEARCMD 0xb2 //设置电机电流档位 1-32
#define SETREFVOLCMD 0xb3 //设置参考电压 0 - 低 1 - 高
#define SETSMARTCURR 0xb4 //设置智能电流控制值 0 - 设置电流值1/2; 1 - 设置电流值1/4

#define BATCHCONFCMD 0xca //批量配置
#define BATCHCMDNUMB 0x0a //批量配置参数命令数目
#define CMDDATANUM   0x0a //命令字节数
#define DATANUMBER   0x04 //数据字节数

#define EQUIPID      0x02 //设备ID
*/

#define CMPCURRHIGH  6.2   //最大电流档位值(A) 0.31V/0.05Om
#define CMPCURRLOW   3.3   //最小电流档位值(A) 0.165V/0.05Om

namespace Ui {
class ConfigDialog;
}

class ConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConfigDialog(QWidget *parent = 0);
    ~ConfigDialog();
    void receiveDate(const QByteArray &data);

    enum SaveFormat {
        Json, Binary
    };

    enum PaneType {
        SMI57XXXX=0, SMI42XXXX
    };

    bool loadConfigFile(SaveFormat saveFormat);
    bool saveConfigFile(SaveFormat saveFormat) const;

    struct Configs {
        int elecCtrl; //待机电流
        //int volLevel; //参考电压 310mv - 0 165mv - 1
        double elecGrade; //额定电流
        int elecLevel; //电机细分等级
        int codeType; //逻辑编码方向
        int plusType;  //脉冲方向
        int maxN; //负向最大允许位移
        int maxP; //正向最大允许位移
        int decTime; //减速时间
        int accTime; //加速时间
        int rsBaud;  //RS485波特率
        int canBaud; //CAN波特率
        int deviceId; //设备ID
        int motorDirect; //电机逻辑正方向 0-顺时针，1-逆时针
        int circleLen;  //外轮圆周长
        double maxCurr; //最大电流
        int countOut;    //输出端口数
        int countIn;    //输入端口数
        PaneType pane;  //板型
    };

    Configs configs() const;
    void tip(); //提示重新载入软件

signals:
    void sendConfig(const QByteArray &data);

private slots:
    void on_writeSerialBtn_clicked();

    void on_saveConfigBtn_clicked();

    void on_readConfigBtn_clicked();

    void on_resetBtn_clicked();

private:
    quint32 power(int index); //return 2^index

    void convert(int data, quint8 *p, int size); //convert int to quint8* with size
    void compact(quint8 *p, QByteArray &data, int size); //convert quint8* to QByteArray with size

    void read(const QJsonObject &json); //读取json文件
    void write(QJsonObject &json) const; //写入json文件

    void data(const Configs *configs, QByteArray &qa); //配置结构转换为QByteArray

    void updateConfigs();

    int level(double base, double cmp); //计算电流档位

private:
    void initUI();
    void initConnect();

private:
    Ui::ConfigDialog *ui;
    QMap<QString, double> configDatas;
    Configs currentConfigs;
    int initId;
};

#endif // CONFIGDIALOG_H
