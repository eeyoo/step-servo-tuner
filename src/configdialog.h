#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QDialog>
#include <QJsonObject>
#include <QMap>

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
        SMI57XXXX, SMI42XXXX
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
        int leftLimitEn; //左限位使能
        int leftLimitCfg; //左限位设置
        int rightLimitEn; //右限位使能
        int rightLimitCfg;//右限位设置
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

    void setUI();
    void setData();

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
