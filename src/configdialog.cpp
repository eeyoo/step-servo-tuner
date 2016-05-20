#include "configdialog.h"
#include "ui_configdialog.h"
#include "command.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QMessageBox>

#include <QtCore/QDebug>

ConfigDialog::ConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigDialog)
{
    ui->setupUi(this);
    initUI();
    updateConfigs();
    initId = configDatas["device_id"];
}

ConfigDialog::~ConfigDialog()
{
    delete ui;
}

int ConfigDialog::level(double base, double cmp)
{

    //根据额定电流值判断电压参考值和电流档位

    int level = 1;
    double e = 0.0;

    for(int i=0; i<32; i++) {
        //qDebug() << QString("level %1, e %2").arg(level).arg(e);
        e = (level*base/32) - cmp;
        if(e > 0)
            break;
        level++;
    }

    return level-1; //0-31

}

void ConfigDialog::on_writeSerialBtn_clicked()
{
    updateConfigs(); //更新Map struct

    int n = currentConfigs.elecLevel;
    int temp[] = {1,2,4,8,16,32,64,128,256};

    int div = temp[n];  //电机细分等级
    int param = 200*div / currentConfigs.circleLen; //位移脉冲转换值



    quint8 id[2];
    convert(initId, id, 2);
    //智能电流控制
    quint8 pec[4];
    convert(currentConfigs.elecCtrl,pec,4);
    quint8 ec[] = {id[0],id[1],SETSMARTCURR,0x00,0x01,pec[0],pec[1],pec[2],pec[3],0x00};
    QByteArray qEC;
    compact(ec, qEC, 10);

    //参考电压边界与电流档位需要额定电流与板型决定
    int volLevel = 0; // 0 or 1
    int elecLevel = 0; // 0 - 31
    double amp = currentConfigs.elecGrade; //额定电流值
    double maxCurr = currentConfigs.maxCurr; //最大电流限制
    if(amp > CMPCURRLOW)
    {
        volLevel = 0; //310mv
        elecLevel = level(CMPCURRHIGH, maxCurr);
    } else {
        volLevel = 1; //165mv
        elecLevel = level(CMPCURRLOW, amp);
    }

    //参考电压
    quint8 pvl[4];
    convert(volLevel, pvl, 4);
    quint8 vl[] = {id[0],id[1],SETREFVOLCMD,0x00,0x02,pvl[0],pvl[1],pvl[2],pvl[3],0x00};
    QByteArray qVL;
    compact(vl, qVL, 10);
    //电流档位
    quint8 peg[4];
    convert(elecLevel, peg, 4);
    quint8 eg[] = {id[0],id[1],SETCUGEARCMD,0x00,0x03,peg[0],peg[1],peg[2],peg[3],0x00};
    QByteArray qEG;
    compact(eg, qEG, 10);
    //电机细分等级    
    quint8 pel[4];
    convert(div, pel, 4);
    QByteArray qEL;
    quint8 el[] = {id[0],id[1],SETMOTDIVCMD,0x00,0x04,pel[0],pel[1],pel[2],pel[3],0x00};
    compact(el, qEL, 10);
    //电机逻辑方向
    quint8 pmd[4];
    convert(currentConfigs.motorDirect, pmd, 4);
    quint8 md[] = {id[0],id[1],SETMOTORDIRE,0x01,0x05,pmd[0],pmd[1],pmd[2],pmd[3],0x00};
    QByteArray qMD;
    compact(md, qMD, 10);
    //配置设备ID
    quint8 pid[4];
    convert(currentConfigs.deviceId, pid, 4);
    QByteArray qID;
    quint8 did[] = {id[0],id[1],SETDEVICEID,0x00,0x06,pid[0],pid[1],pid[2],pid[3],0x00};
    compact(did, qID, 10);
    //CAN
    quint8 pcan[4];
    convert(currentConfigs.canBaud+1, pcan, 4);
    QByteArray qCAN;
    quint8 can[] = {id[0],id[1],SETCANBAUDRT,0x00,0x07,pcan[0],pcan[1],pcan[2],pcan[3],0x00};
    compact(can, qCAN, 10);
    //RS485
    quint8 prs[4];
    convert(currentConfigs.rsBaud+1,prs,4);
    QByteArray qRS;
    quint8 rs[] = {id[0],id[1],SETSERBAUDRT,0x00,0x08,prs[0],prs[1],prs[2],prs[3],0x00};
    compact(rs,qRS,10);
    //逻辑编码方向
    quint8 pct[4];
    convert(currentConfigs.codeType,pct,4);
    QByteArray qCT;
    quint8 ct[] = {id[0],id[1],SETCODELOGDI,0x00,0x09,pct[0],pct[1],pct[2],pct[3],0x00};
    compact(ct,qCT,10);
    //脉冲控制方式
    quint8 ppt[4];
    convert(currentConfigs.plusType,ppt,4);
    QByteArray qPT;
    quint8 pt[] = {id[0],id[1],SETPLUSCTRLT,0x00,0x0a,ppt[0],ppt[1],ppt[2],ppt[3],0x00};
    compact(pt,qPT,10);
    //负向最大允许位移
    quint8 pmn[4];
    convert(param*currentConfigs.maxN,pmn,4);
    QByteArray qMN;
    quint8 mn[] = {id[0],id[1],SETNEGMAXPOS,0x00,0x0b,pmn[0],pmn[1],pmn[2],pmn[3],0x00};
    compact(mn,qMN,10);
    //正向最大允许位移
    quint8 pmp[4];
    convert(param*currentConfigs.maxP,pmp,4);
    QByteArray qMP;
    quint8 mp[] = {id[0],id[1],SETPOSMAXPOS,0x00,0x0c,pmp[0],pmp[1],pmp[2],pmp[3],0x00};
    compact(mp,qMP,10);
    //减速时间
    quint8 pdt[4];
    convert(currentConfigs.decTime,pdt,4);
    QByteArray qDT;
    quint8 dt[] = {id[0],id[1],SETDECDURCMD,0x00,0x0d,pdt[0],pdt[1],pdt[2],pdt[3],0x00};
    compact(dt,qDT,10);
    //加速时间
    quint8 pat[4];
    convert(currentConfigs.accTime,pat,4);
    QByteArray qAT;
    quint8 at[] = {id[0],id[1],SETACCDURCMD,0x00,0x0e,pat[0],pat[1],pat[2],pat[3],0x00};
    compact(at,qAT,10);
    //组合指令数据
    QByteArray qHD;
    quint8 hd[] = {id[0],id[1],BATCHCONFCMD,0x00,0x01,0x0e,0x00,0x00,0x00,0x00};
    compact(hd,qHD,10);

    QByteArray data;

    data.append(qHD).append(qAT).append(qDT).append(qMP).append(qMN).append(qPT)
            .append(qCT).append(qRS).append(qCAN)
            .append(qID).append(qMD).append(qEL).append(qEG).append(qVL).append(qEC);

    //data.append(qVL).append(qEG);

    emit sendConfig(data);

    saveConfigFile(Json);
    //tip();
}

quint32 ConfigDialog::power(int index)
{
    quint32 ret = 1;
    while(index--) {
        ret = ret * 2;
    }
    return ret;
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

void ConfigDialog::receiveDate(const QByteArray &data)
{
    qDebug() << data;
}

void ConfigDialog::on_saveConfigBtn_clicked()
{
    updateConfigs();
/*
    configDatas["elec_ctrl"] = currentConfigs.elecCtrl;
    configDatas["vol_level"] = currentConfigs.volLevel;
    configDatas["elec_grade"] = currentConfigs.elecGrade;
    configDatas["elec_level"] = currentConfigs.elecLevel;
    configDatas["code_type"] = currentConfigs.codeType;
    configDatas["plus_type"] = currentConfigs.plusType;
    configDatas["negative_max"] = currentConfigs.maxN;
    configDatas["positive_max"] = currentConfigs.maxP;
    configDatas["dec_time"] = currentConfigs.decTime;
    configDatas["acc_time"] = currentConfigs.accTime;
    configDatas["rs485_baud"] = currentConfigs.rsBaud;
    configDatas["can_baud"] = currentConfigs.canBaud;
    configDatas["device_id"] = currentConfigs.deviceId;
    configDatas["motor_direct"] = currentConfigs.motorDirect;
    configDatas["circle_len"] = currentConfigs.circleLen;
*/

    saveConfigFile(Json);

    //tip();

}

void ConfigDialog::updateConfigs()
{
    //串口下载参数
    int elecCtrl = ui->elecContrl->currentIndex(); //智能电流控制
    //int volLevel = ui->volLevel->currentIndex();    //参考电压
    double elecGrade = ui->elecGrade->value();      //电流档位
    int elecLevel = ui->motorLevel->currentIndex(); //电机细分等级
    //int codeType = ui->codeLogicDirect->currentIndex(); //逻辑编码方向
    int codeType = 0;
    int plusType = ui->plusType->currentIndex(); //脉冲控制方式
    int maxN = ui->maximumNegative->value(); //负向最大允许位移
    int maxP = ui->maximumPositive->value(); //正向最大允许位移
    int decTime = ui->servoDecTime->value(); //减速时间
    int accTime = ui->servoAccTime->value(); //加速时间
    int rsBaud = ui->rs485Baud->currentIndex(); //RS485波特率
    int canBaud = ui->canBaud->currentIndex();  //CAN波特率
    int deviceID = ui->deviceID->value();   //设备ID
    int motorDirect = ui->motorDirect->currentIndex();  //电机逻辑正方向
    //不需下载参数
    int circe = ui->circleLen->value(); //电机外轮圆周长
    int pane = ui->paneType->currentIndex(); //板型（0 - 57mm 1 - 42mm）

    //保存下载参数
    currentConfigs.elecCtrl = elecCtrl;
    //currentConfigs.volLevel = volLevel;
    currentConfigs.elecGrade = elecGrade;
    currentConfigs.elecLevel = elecLevel;
    currentConfigs.codeType = codeType;
    currentConfigs.plusType = plusType;
    currentConfigs.maxN = maxN;
    currentConfigs.maxP = maxP;
    currentConfigs.decTime = decTime;
    currentConfigs.accTime = accTime;
    currentConfigs.rsBaud = rsBaud;
    currentConfigs.canBaud = canBaud;
    currentConfigs.deviceId = deviceID;
    currentConfigs.motorDirect = motorDirect;
    currentConfigs.circleLen = circe;
    if(pane == 0) {
        currentConfigs.pane = SMI57XXXX;
        currentConfigs.maxCurr = 4.5;
        currentConfigs.countIn = 5;
        currentConfigs.countOut = 1;
    } else if(pane == 1) {
        currentConfigs.pane = SMI42XXXX;
        currentConfigs.maxCurr = 3.0;
        currentConfigs.countIn = 3;
        currentConfigs.countOut = 1;
    }

    configDatas["elec_ctrl"] = currentConfigs.elecCtrl;
    //configDatas["vol_level"] = currentConfigs.volLevel;
    configDatas["elec_grade"] = currentConfigs.elecGrade;
    configDatas["elec_level"] = currentConfigs.elecLevel;
    configDatas["code_type"] = currentConfigs.codeType;
    configDatas["plus_type"] = currentConfigs.plusType;
    configDatas["negative_max"] = currentConfigs.maxN;
    configDatas["positive_max"] = currentConfigs.maxP;
    configDatas["dec_time"] = currentConfigs.decTime;
    configDatas["acc_time"] = currentConfigs.accTime;
    configDatas["rs485_baud"] = currentConfigs.rsBaud;
    configDatas["can_baud"] = currentConfigs.canBaud;
    configDatas["device_id"] = currentConfigs.deviceId;
    configDatas["motor_direct"] = currentConfigs.motorDirect;
    configDatas["circle_len"] = currentConfigs.circleLen;
    configDatas["max_current"] = currentConfigs.maxCurr;
    configDatas["count_out"] = currentConfigs.countOut;
    configDatas["count_in"] = currentConfigs.countIn;
    configDatas["pane_type"] = currentConfigs.pane;
}

ConfigDialog::Configs ConfigDialog::configs() const
{
    return currentConfigs;
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
    QMapIterator<QString, double> i(configDatas);
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
    loadConfigFile(Json);

    ui->deviceID->setValue(configDatas["device_id"]);
    ui->rs485Baud->setCurrentIndex(configDatas["rs485_baud"]);
    ui->canBaud->setCurrentIndex(configDatas["can_baud"]);
    ui->elecContrl->setCurrentIndex(configDatas["elec_ctrl"]);
    //ui->volLevel->setCurrentIndex(configDatas["vol_level"]);
    ui->motorLevel->setCurrentIndex(configDatas["elec_level"]);
    //ui->codeLogicDirect->setCurrentIndex(configDatas["code_type"]);
    ui->plusType->setCurrentIndex(configDatas["plus_type"]);
    ui->elecGrade->setValue(configDatas["elec_grade"]);
    ui->maximumNegative->setValue(configDatas["negative_max"]);
    ui->maximumPositive->setValue(configDatas["positive_max"]);
    ui->servoAccTime->setValue(configDatas["acc_time"]);
    ui->servoDecTime->setValue(configDatas["dec_time"]);
    ui->circleLen->setValue(configDatas["circle_len"]);
    ui->paneType->setCurrentIndex(configDatas["pane_type"]);
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

    for (int i=0; i < configArray.size(); i++) {
        QJsonObject obj = configArray[i].toObject();
        QString name = obj["name"].toString();
        double value = obj["value"].toDouble();
        configDatas.insert(name, value);
    }

}

void ConfigDialog::initUI()
{

    loadConfigFile(Json);

    ui->deviceID->setValue(configDatas["device_id"]);
    ui->rs485Baud->setCurrentIndex(configDatas["rs485_baud"]);
    ui->canBaud->setCurrentIndex(configDatas["can_baud"]);
    ui->elecContrl->setCurrentIndex(configDatas["elec_ctrl"]);
    //ui->volLevel->setCurrentIndex(configDatas["vol_level"]);
    ui->motorLevel->setCurrentIndex(configDatas["elec_level"]);
    //ui->codeLogicDirect->setCurrentIndex(configDatas["code_type"]);
    ui->plusType->setCurrentIndex(configDatas["plus_type"]);
    ui->elecGrade->setValue(configDatas["elec_grade"]);
    ui->maximumNegative->setValue(configDatas["negative_max"]);
    ui->maximumPositive->setValue(configDatas["positive_max"]);
    ui->servoAccTime->setValue(configDatas["acc_time"]);
    ui->servoDecTime->setValue(configDatas["dec_time"]);
    ui->circleLen->setValue(configDatas["circle_len"]);
    ui->paneType->setCurrentIndex(configDatas["pane_type"]);

}

void ConfigDialog::initConnect()
{

}

void ConfigDialog::tip()
{
    QMessageBox::about(this,tr("提示对话框"),tr("修改配置参数后需重启软件生效！"));
}
