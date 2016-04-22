#include "configdialog.h"
#include "ui_configdialog.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

#include <QtCore/QDebug>

ConfigDialog::ConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigDialog)
{
    ui->setupUi(this);

    ui->volLevel->addItem(QWidget::tr("低"));
    ui->volLevel->addItem(QWidget::tr("高"));

    ui->elecContrl->addItem(QWidget::tr("1/2电流值"));
    ui->elecContrl->addItem(QWidget::tr("1/4电流值"));

    QStringList divList;
    divList << "1" << "2" << "4" << "8" << "16" << "32" << "64" << "128" << "256";
    ui->motorLevel->addItems(divList);

    ui->plusType->addItem(QWidget::tr("内部脉冲控制"));
    ui->plusType->addItem(QWidget::tr("外部脉冲控制"));

    ui->codeLogicDirect->addItem(QWidget::tr("负向"));
    ui->codeLogicDirect->addItem(QWidget::tr("正向"));

    QStringList rsBaudList;
    rsBaudList << "115200" << "38400" << "19200" << "9600" << "4800";
    ui->rs485Baud->addItems(rsBaudList);

    QStringList canBaudList;
    canBaudList << "1000K" << "500K" << "250K" << "125K" << "100K" << "50K";
    ui->canBaud->addItems(canBaudList);

    //ui->val1->setText("300");
    //ui->val2->setText("50");
    //ui->val3->setText("5000");
    //默认读取JSON配置文件
    loadConfigFile(Json);
}

ConfigDialog::~ConfigDialog()
{
    delete ui;
}

void ConfigDialog::on_cancelBtn_clicked()
{
    close();
}

void ConfigDialog::on_pushBtn_clicked()
{
    quint32 elecCtrl = ui->elecContrl->currentIndex(); //智能电流控制
    quint32 volLevel = ui->volLevel->currentIndex(); //参考电压
    quint32 elecGrade = ui->elecGrade->text().toUInt(); //电流档位
    quint32 elecLevel = ui->motorLevel->currentIndex(); //电机细分等级
    quint32 codeType = ui->codeLogicDirect->currentIndex(); //逻辑编码方向
    quint32 plusType = ui->plusType->currentIndex(); //脉冲控制方式
    quint32 maxN = ui->maximumNegative->text().toUInt(); //负向最大允许位移
    //qDebug() << elecCtrl;
    quint32 maxP = ui->maximumPositive->text().toUInt(); //正向最大允许位移
    quint32 decTime = ui->servoDecTime->text().toUInt(); //减速时间
    quint32 accTime = ui->servoAccTime->text().toUInt(); //加速时间

    quint32 rsBaud = ui->rs485Baud->currentIndex() + 1; //RS485波特率
    quint32 canBaud = ui->canBaud->currentIndex() + 1;  //CAN波特率
    quint32 deviceID = ui->deviceID->text().toUInt();   //设备ID

    //整型数据如何转换为字节数组
    /*
    quint8 *pEC = convert4bytes(elecCtrl);              //智能电流
    quint8 *pVL = convert4bytes(volLevel);              //参考电压
    quint8 *pEG = convert4bytes(elecGrade);                    //电流档位
    quint8 *pEL = convert4bytes(power(elecLevel));       //电机细分等级
    quint8 *pCT = convert4bytes(codeType); //逻辑编码方向
    quint8 *pPT = convert4bytes(plusType); //脉冲控制方式
    quint8 *pMN = convert4bytes(maxN);       //负向最大允许位移
    quint8 *pMP = convert4bytes(maxP);       //正向最大允许位移
    quint8 *pDT = convert4bytes(decTime);       //减速时间
    quint8 *pAT = convert4bytes(accTime);       //加速时间
    */

    //组合指令数据
    quint8 *da = convert4bytes(BATCHCMDNUMB);
    quint8 head[] = {0x02,0x00,BATCHCONFCMD,0x00,0x00,da[0],da[1],da[2],da[3],0x00};
    //智能电流控制
    QByteArray qEC = raw(convert4bytes(elecCtrl), DATANUMBER);
    quint8 ec[] = {0x02,0x00,SETSMARTCURR,0x00,0x01,qEC[0],qEC[1],qEC[2],qEC[3],0x00};
    //参考电压
    QByteArray qVL = raw(convert4bytes(volLevel), DATANUMBER);
    quint8 vl[] = {0x02,0x00,SETREFVOLCMD,0x00,0x02,qVL[0],qVL[1],qVL[2],qVL[3],0x00};
    //电流档位
    QByteArray qEG = raw(convert4bytes(elecGrade), DATANUMBER);
    quint8 eg[] = {0x02,0x00,SETCUGEARCMD,0x00,0x03,qEG[0],qEG[1],qEG[2],qEG[3],0x00};
    //电机细分等级
    QByteArray qEL = raw(convert4bytes(power(elecLevel)), DATANUMBER);
    quint8 el[] = {0x02,0x00,SETMOTDIVCMD,0x00,0x04,qEL[0],qEL[1],qEL[2],qEL[3],0x00};
    //逻辑编码方向
    QByteArray qCT = raw(convert4bytes(codeType), DATANUMBER);
    quint8 ct[] = {0x02,0x00,SETCODELOGDI,0x00,0x05,qCT[0],qCT[1],qCT[2],qCT[3],0x00};
    //脉冲控制方式
    QByteArray qPT = raw(convert4bytes(plusType), DATANUMBER);
    quint8 pt[] = {0x02,0x00,SETPLUSCTRLT,0x00,0x06,qPT[0],qPT[1],qPT[2],qPT[3],0x00};
    //负向最大允许位移
    QByteArray qMN = raw(convert4bytes(maxN), DATANUMBER);
    quint8 mn[] = {0x02,0x00,SETNEGMAXPOS,0x00,0x07,qMN[0],qMN[1],qMN[2],qMN[3],0x00};
    //正向最大允许位移
    QByteArray qMP = raw(convert4bytes(maxP), DATANUMBER);
    quint8 mp[] = {0x02,0x00,SETPOSMAXPOS,0x00,0x08,qMP[0],qMP[1],qMP[2],qMP[3],0x00};
    //减速时间
    QByteArray qDT = raw(convert4bytes(decTime), DATANUMBER);
    quint8 dt[] = {0x02,0x00,SETDECDURCMD,0x00,0x09,qDT[0],qDT[1],qDT[2],qDT[3],0x00};
    //加速时间
    QByteArray qAT = raw(convert4bytes(accTime), DATANUMBER);
    quint8 at[] = {0x02,0x00,SETACCDURCMD,0x00,0x0a,qAT[0],qAT[1],qAT[2],qAT[3],0x00};

    QByteArray hd = raw(head, BATCHCMDNUMB);
    QByteArray qa1 = raw(ec, BATCHCMDNUMB);
    QByteArray qa2 = raw(vl, BATCHCMDNUMB);
    QByteArray qa3 = raw(eg, BATCHCMDNUMB);
    QByteArray qa4 = raw(el, BATCHCMDNUMB);
    QByteArray qa5 = raw(ct, BATCHCMDNUMB);
    QByteArray qa6 = raw(pt, BATCHCMDNUMB);
    QByteArray qa7 = raw(mn, BATCHCMDNUMB);
    QByteArray qa8 = raw(mp, BATCHCMDNUMB);
    QByteArray qa9 = raw(dt, BATCHCMDNUMB);
    QByteArray qa10 = raw(at, BATCHCMDNUMB);

    QByteArray data;
    data.append(hd).append(qa10).append(qa9).append(qa8).append(qa7).append(qa6)
            .append(qa5).append(qa4).append(qa3).append(qa2).append(qa1);
    //data.append(qa2);

    qDebug() << data.toHex().toUpper();
    emit sendData(data);
}

quint32 ConfigDialog::power(int index)
{
    quint32 ret = 1;
    while(index--) {
        ret = ret * 2;
    }
    return ret;
}

quint8* ConfigDialog::convert4bytes(const quint32 n)
{
    static quint8 temp[4];
    temp[0] = n;
    temp[1] = (n >> 8);
    temp[2] = (n >> 16);
    temp[3] = (n >> 24);

    return temp;
}

quint8* ConfigDialog::convert2bytes(const quint32 n)
{
    static quint8 temp[2];
    temp[0] = n;
    temp[1] = (n >> 8);

    return temp;
}

QByteArray ConfigDialog::raw(quint8 *p, int size)
{
    QByteArray qa;
    for(int i=0; i<size; i++)
    {
        qa[i] = p[i];
    }
    return qa;
}

void ConfigDialog::receiveDate(const QByteArray &data)
{
    qDebug() << data;
}

void ConfigDialog::on_write2file_clicked()
{
    //获取界面输入参数字符串
    int val1 = ui->val1->text().toInt();
    int val2 = ui->val2->text().toInt();
    int val3 = ui->val3->text().toInt();
    //写入文件便于采用
    configDatas["circle_len"] = val1;
    configDatas["step_len"] = val2;
    configDatas["ctrl_feq"] = val3;

    bool ret = saveConfigFile(Json);
    if(ret) {
        qDebug() << "save config file successful.";
        close();
    } else
        qDebug() << "can't save config file.";

}

bool ConfigDialog::saveConfigFile(SaveFormat saveFormat) const
{
    QFile saveFile(saveFormat == Json
                   ? QStringLiteral("save.json")
                   : QStringLiteral("save.dat"));
    if(!saveFile.open(QIODevice::WriteOnly)) {
        qWarning("不能打开文件!");
        return false;
    }

    QJsonObject configObject;
    write(configObject);
    QJsonDocument saveDoc(configObject);
    saveFile.write(saveFormat == Json
                   ? saveDoc.toJson()
                   : saveDoc.toBinaryData());
    return true;
}

void ConfigDialog::write(QJsonObject &json) const
{
    QJsonArray configArray;
    QMapIterator<QString, int> i(configDatas);
    while(i.hasNext()) {
        i.next();
        QJsonObject obj;
        obj["name"] = i.key();
        obj["value"] = i.value();
        configArray.append(obj);
    }

    json["configs"] = configArray;
}

void ConfigDialog::on_read4file_clicked()
{
    //读取磁盘文件
    bool ret = loadConfigFile(Json);
    if (ret) {
        qDebug() << "write to json file successful.";
    } else {
        qDebug() << "write to json file failed.";
    }


}

bool ConfigDialog::loadConfigFile(SaveFormat saveFormat)
{
    QFile loadFile(saveFormat == Json
                   ? QStringLiteral("save.json")
                   : QStringLiteral("save.dat"));
    if(!loadFile.open(QIODevice::ReadOnly)) {
        qWarning("不能打开文件!");
        return false;
    }

    QByteArray saveData = loadFile.readAll();

    QJsonDocument loadDoc(saveFormat == Json
                          ? QJsonDocument::fromJson(saveData)
                          : QJsonDocument::fromBinaryData(saveData));

    read(loadDoc.object()); //解析QJsonDocument文件

    return true;
}

void ConfigDialog::read(const QJsonObject &json)
{
    //JSON格式文件解析出来
    QJsonArray configArray = json["configs"].toArray();

    //ui->val1->setText(configArray[]);

    for (int i=0; i < configArray.size(); i++) {
        QJsonObject obj = configArray[i].toObject();
        QString name = obj["name"].toString();
        int value = obj["value"].toInt();
        configDatas.insert(name, value);
        //configDatas.key(obj["name"]);
        //configDatas.value(obj["value"]);
    }
    //界面显示出来
    ui->val1->setText(QString::number(configDatas["circle_len"]));
    ui->val2->setText(QString::number(configDatas["step_len"]));
    ui->val3->setText(QString::number(configDatas["ctrl_feq"]));

}
