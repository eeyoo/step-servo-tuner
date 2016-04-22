#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QDialog>
#include <QJsonObject>
#include <QMap>

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

    bool loadConfigFile(SaveFormat saveFormat);
    bool saveConfigFile(SaveFormat saveFormat) const;

signals:
    void sendData(const QByteArray &data);

private slots:
    void on_cancelBtn_clicked();

    void on_pushBtn_clicked();

    void on_write2file_clicked();

    void on_read4file_clicked();

private:
    quint8* convert4bytes(const quint32); //quint32 -> quint8[4]
    quint8* convert2bytes(const quint32); //quint32 -> quint8[2]
    quint32 power(int index); //return 2^index
    QByteArray raw(quint8 *p, int size); //quint8[] -> QByteArray

    void read(const QJsonObject &json); //读取json文件
    void write(QJsonObject &json) const; //写入json文件
private:
    Ui::ConfigDialog *ui;
    QMap<QString, int> configDatas;
};

#endif // CONFIGDIALOG_H
