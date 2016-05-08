#include "configdialog.h"
#include "ui_configdialog.h"
#include "command.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

#include <QtCore/QDebug>

ConfigDialog::ConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigDialog)
{
    initUI();
}

ConfigDialog::~ConfigDialog()
{
    delete ui;
}

void ConfigDialog::on_writeConfigBtn_clicked()
{
    int elecCtrl = ui->elecContrl->currentIndex(); //智能电流控制
    int volLevel = ui->volLevel->currentIndex(); //参考电压
    int elecGrade = ui->elecGrade->text().toInt(); //电流档位
    int elecLevel = ui->motorLevel->currentIndex(); //电机细分等级
    int codeType = ui->codeLogicDirect->currentIndex(); //逻辑编码方向
    int plusType = ui->plusType->currentIndex(); //脉冲控制方式
    int maxN = ui->maximumNegative->text().toInt(); //负向最大允许位移
    int maxP = ui->maximumPositive->text().toUInt(); //正向最大允许位移
    int decTime = ui->servoDecTime->text().toUInt(); //减速时间
    int accTime = ui->servoAccTime->text().toUInt(); //加速时间

    int rsBaud = ui->rs485Baud->currentIndex() + 1; //RS485波特率
    int canBaud = ui->canBaud->currentIndex() + 1;  //CAN波特率
    int deviceID = ui->deviceID->text().toUInt();   //设备ID
    int motorDirect = ui->motorDirect->currentIndex();  //电机逻辑正方向


    //QByteArray data;
    quint8 id[2];
    convert(deviceID, id, 2);
    //智能电流控制
    QByteArray qEC = raw(convert4bytes(elecCtrl), DATANUMBER);
    quint8 ec[] = {id[0],id[1],SETSMARTCURR,0x00,0x01,qEC[0],qEC[1],qEC[2],qEC[3],0x00};
    //参考电压
    QByteArray qVL = raw(convert4bytes(volLevel), DATANUMBER);
    quint8 vl[] = {id[0],id[1],SETREFVOLCMD,0x00,0x02,qVL[0],qVL[1],qVL[2],qVL[3],0x00};
    //电流档位
    QByteArray qEG = raw(convert4bytes(elecGrade), DATANUMBER);
    quint8 eg[] = {id[0],id[1],SETCUGEARCMD,0x00,0x03,qEG[0],qEG[1],qEG[2],qEG[3],0x00};
    //电机细分等级
    QByteArray qEL = raw(convert4bytes(power(elecLevel)), DATANUMBER);
    quint8 el[] = {id[0],id[1],SETMOTDIVCMD,0x00,0x04,qEL[0],qEL[1],qEL[2],qEL[3],0x00};
    //电机逻辑方向
    quint8 pmd[4];
    convert(motorDirect, pmd, 4);
    quint8 pMD[] = {id[0],id[1],SETMOTORDIRE,0x01,0x05,pmd[0],pmd[1],pmd[2],pmd[3],0x00};
    QByteArray qmd;
    compact(pMD, qmd, 10);
    //配置设备ID
    QByteArray qID = raw(convert4bytes(deviceID), DATANUMBER);
    quint8 did[] = {id[0],id[1],SETDEVICEID,0x00,0x06,qID[0],qID[1],qID[2],qID[3],0x00};
    //CAN
    QByteArray qCAN = raw(convert4bytes(canBaud), DATANUMBER);
    quint8 can[] = {id[0],id[1],SETCANBAUDRT,0x00,0x07,qCAN[0],qCAN[1],qCAN[2],qCAN[3],0x00};
    //RS485
    QByteArray qRS = raw(convert4bytes(rsBaud), DATANUMBER);
    quint8 rs[] = {id[0],id[1],SETSERBAUDRT,0x00,0x08,qRS[0],qRS[1],qRS[2],qRS[3],0x00};
    //逻辑编码方向
    QByteArray qCT = raw(convert4bytes(codeType), DATANUMBER);
    quint8 ct[] = {id[0],id[1],SETCODELOGDI,0x00,0x09,qCT[0],qCT[1],qCT[2],qCT[3],0x00};
    //脉冲控制方式
    QByteArray qPT = raw(convert4bytes(plusType), DATANUMBER);
    quint8 pt[] = {id[0],id[1],SETPLUSCTRLT,0x00,0x0a,qPT[0],qPT[1],qPT[2],qPT[3],0x00};

    //负向最大允许位移
    QByteArray qMN = raw(convert4bytes4negative(-maxN), DATANUMBER);
    quint8 mn[] = {id[0],id[1],SETNEGMAXPOS,0x00,0x0b,qMN[0],qMN[1],qMN[2],qMN[3],0x00};
    //正向最大允许位移
    QByteArray qMP = raw(convert4bytes(maxP), DATANUMBER);
    quint8 mp[] = {id[0],id[1],SETPOSMAXPOS,0x00,0x0c,qMP[0],qMP[1],qMP[2],qMP[3],0x00};
    //减速时间
    QByteArray qDT = raw(convert4bytes(decTime), DATANUMBER);
    quint8 dt[] = {id[0],id[1],SETDECDURCMD,0x00,0x0d,qDT[0],qDT[1],qDT[2],qDT[3],0x00};
    //加速时间
    QByteArray qAT = raw(convert4bytes(accTime), DATANUMBER);
    quint8 at[] = {id[0],id[1],SETACCDURCMD,0x00,0x0e,qAT[0],qAT[1],qAT[2],qAT[3],0x00};
    //组合指令数据
    quint8 head[] = {id[0],id[1],BATCHCONFCMD,0x00,0x01,0x0f,0x00,0x00,0x00,0x00};

    QByteArray qa1 = raw(ec, BATCHCMDNUMB);
    QByteArray qa2 = raw(vl, BATCHCMDNUMB);
    QByteArray qa3 = raw(eg, BATCHCMDNUMB);
    QByteArray qa4 = raw(el, BATCHCMDNUMB);
    QByteArray qa5 = raw(did, BATCHCMDNUMB);
    QByteArray qa6 = raw(can, BATCHCMDNUMB);
    QByteArray qa7 = raw(rs, BATCHCMDNUMB);
    QByteArray qa8 = raw(ct, BATCHCMDNUMB);
    QByteArray qa9 = raw(pt, BATCHCMDNUMB);
    QByteArray qa10 = raw(mn, BATCHCMDNUMB);
    QByteArray qa11 = raw(mp, BATCHCMDNUMB);
    QByteArray qa12 = raw(dt, BATCHCMDNUMB);
    QByteArray qa13 = raw(at, BATCHCMDNUMB);
    QByteArray hd = raw(head, BATCHCMDNUMB);

    QByteArray data;
    data.append(hd).append(qa13).append(qa12).append(qa11).append(qa10).append(qa9)
            .append(qa8).append(qa7).append(qa6)
            .append(qa5).append(qmd).append(qa4).append(qa3).append(qa2).append(qa1);

    //data.append(qmd);

    qDebug() << data.toHex().toUpper();
    emit sendData(data);
    close();
}

void ConfigDialog::data(const Configs *configs, QByteArray &qa)
{

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

void ConfigDialog::convert(int data, quint8 *p, int size)
{
    for(int i=0; i < size; i++) {
        p[i] = data >> (8*i);
    }
}

void ConfigDialog::compact(quint8 *p, QByteArray &data, int size)
{
    for(int i=0; i<size; i++) {
        data[i] = p[i];
    }
}

quint8* ConfigDialog::convert4bytes4negative(const int n)
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

void ConfigDialog::on_saveConfigBtn_clicked()
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
        qDebug() << "write json file successful.";
        close();
    } else
        qDebug() << "write json file failed.";

}

bool ConfigDialog::saveConfigFile(SaveFormat saveFormat) const
{
    QFile saveFile(saveFormat == Json
                   ? QStringLiteral("save.json")
                   : QStringLiteral("save.dat"));
    if(!saveFile.open(QIODevice::WriteOnly)) {
        //qWarning("不能打开文件!");
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

void ConfigDialog::on_readConfigBtn_clicked()
{
    //读取磁盘文件
    bool ret = loadConfigFile(Json);
    if (ret) {
        qDebug() << "read json file successful.";
    } else {
        qDebug() << "read json file failed.";
    }


}

bool ConfigDialog::loadConfigFile(SaveFormat saveFormat)
{
    QFile loadFile(saveFormat == Json
                   ? QStringLiteral("save.json")
                   : QStringLiteral("save.dat"));
    if(!loadFile.open(QIODevice::ReadOnly)) {
        //qWarning("不能打开文件!");
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
    }
    //界面显示出来
    ui->val1->setText(QString::number(configDatas["circle_len"]));
    ui->val2->setText(QString::number(configDatas["step_len"]));
    ui->val3->setText(QString::number(configDatas["ctrl_feq"]));

}

void ConfigDialog::initUI()
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
    rsBaudList << "115200" << "57600" << "38400" << "19200" << "9600" << "4800";
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

void ConfigDialog::initConnect()
{

}
