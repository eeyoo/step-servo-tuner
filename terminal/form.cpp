#include "form.h"
#include "ui_form.h"

#include "command.h"
#include "absmovecmd.h"
#include <iostream>

#include <QtCore/QDebug>

Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);
    /*
    console = new Console();
    console->setEnabled(false);
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(console);
    ui->tab_terminal->setLayout(layout);
    */
    initUI();
    initConnect();
}

Form::~Form()
{
    delete ui;
}

void Form::about()
{

}

void Form::initUI()
{
    ui->backHomeDirection->addItem(QWidget::tr("正向"));
    ui->backHomeDirection->addItem(QWidget::tr("反向"));

    ui->backHomeGear->addItem(QWidget::tr("快档"));
    ui->backHomeGear->addItem(QWidget::tr("慢档"));

    ui->volLevel->addItem(QWidget::tr("低"));
    ui->volLevel->addItem(QWidget::tr("高"));

    ui->elecContrl->addItem(QWidget::tr("1/2电流值"));
    ui->elecContrl->addItem(QWidget::tr("1/4电流值"));

    QStringList list;
    list << "1" << "2" << "4" << "8" << "16" << "32" << "64" << "128" << "256";
    ui->motorLevel->addItems(list);

    ui->plusType->addItem(QWidget::tr("内部脉冲控制"));
    ui->plusType->addItem(QWidget::tr("外部脉冲控制"));

    ui->codeLogicDirect->addItem(QWidget::tr("负向"));
    ui->codeLogicDirect->addItem(QWidget::tr("正向"));
}

void Form::emergencyStop()
{

    quint32 elecLevel = ui->motorLevel->currentIndex(); //电机细分等级
    quint32 volLevel = ui->volLevel->currentIndex(); //参考电压
    quint32 elecCtrl = ui->elecContrl->currentIndex(); //智能电流控制
    quint32 plusType = ui->plusType->currentIndex(); //脉冲方式
    quint32 codeType = ui->codeLogicDirect->currentIndex(); //逻辑编码方向

    QString elecGrade = ui->elecGrade->text(); //电流档位
    QString accTime = ui->servoAccTime->text(); //加速时间
    QString decTime = ui->servoDecTime->text(); //减速时间    
    QString maxP = ui->maximumPositive->text(); //正向最大允许位移
    QString maxN = ui->maximumNegative->text(); //负向最大允许位移

    //整型数据
    quint32 eL = power(elecLevel);
    quint32 eG = elecGrade.toUInt();
    quint32 aT = accTime.toUInt();
    quint32 dT = decTime.toUInt();
    quint32 mP = maxP.toUInt();
    quint32 mN = maxN.toUInt();

    //整型数据如何转换为字节数组
    quint8 *pEL = convert4bytes(eL);       //电机细分等级
    quint8 *pEG = convert4bytes(eG);       //电流档位
    quint8 *pAT = convert4bytes(aT);       //加速时间
    quint8 *pDT = convert4bytes(dT);       //减速时间
    quint8 *pVL = convert4bytes(volLevel); //参考电压
    quint8 *pEC = convert4bytes(elecCtrl); //智能电流
    quint8 *pMP = convert4bytes(mP);       //正向最大允许位移
    quint8 *pMN = convert4bytes(mN);       //负向最大允许位移
    quint8 *pPT = convert4bytes(plusType); //脉冲控制方式
    quint8 *pCT = convert4bytes(codeType); //逻辑编码方向

    //结构体指令数据
    //Cmd c1, c2, c3, c4, c5, c6, c7;

    /*
    char c1[10] = {0x00,0x00,SETMOTDIVCMD_ID,0x00,0x00,pEL[0],pEL[1],pEL[2],pEL[3],0xaa};
    char c2[10] = {0x00,0x00,SETCUGEARCMD_ID,0x00,0x00,pEG[0],pEG[1],pEG[2],pEG[3],0x00};
    char c3[10] = {0x00,0x00,SETACCDURCMD_ID,0x00,0x00,pAT[0],pAT[1],pAT[2],pAT[3],0x00};
    char c4[10] = {0x00,0x00,SETDECDURCMD_ID,0x00,0x00,pDT[0],pDT[1],pDT[2],pDT[3],0x00};
    char c5[10] = {0x00,0x00,SETREFVOLCMD_ID,0x00,0x00,pVL[0],pVL[1],pVL[2],pVL[3],0x00};
    char c6[10] = {0x00,0x00,SETSMARTCURR_ID,0x00,0x00,pEC[0],pEC[1],pEC[2],pEC[3],0x00};
    char c7[10] = {0x00,0x00,SETMAXPOSCMD_ID,0x00,0x00,pMP[0],pMP[1],pMP[2],pMP[3],0x00};
    */
    //结构体转换为QByteArray
    //QByteArray qa1 = QByteArray::fromRawData(c1, sizeof(c1));
    //QByteArray qa2 = raw(c2, 10);
    //QByteArray qa3 = raw(c3, 10);
    //QByteArray qa4 = raw(c4, 10);
    //QByteArray qa5 = raw(c5, 10);
    //QByteArray qa6 = raw(c6, 10);
    //QByteArray qa7 = raw(c7, 10);

    //quint8 *pEL = convert4bytes(eL);
    //qDebug() << *pEL << *(pEL+1) << *(pEL+2) << *(pEL+3);

    QByteArray head; //批量配置参数头部
    head.resize(10);
    head[0] = 0x02;
    head[1] = 0x00;
    head[2] = BATCHCONFCMD;
    head[3] = 0x00;
    head[4] = BATCHCMDNUMB;
    head[5] = 0x00;
    head[6] = 0x00;
    head[7] = 0x00;
    head[8] = 0x00;
    head[9] = 0xff;

    QByteArray data1; //智能电流
    data1.resize(10);
    data1[0] = 0x02;
    data1[1] = 0x00;
    data1[2] = SETSMARTCURR;
    data1[3] = 0x00;
    data1[4] = 0x01;
    data1[5] = *pEC;
    data1[6] = *(pEC+1);
    data1[7] = *(pEC+2);
    data1[8] = *(pEC+3);
    data1[9] = 0xff;

    QByteArray data2; //参考电压
    data2.resize(10);
    data2[0] = 0x02;
    data2[1] = 0x00;
    data2[2] = SETREFVOLCMD;
    data2[3] = 0x00;
    data2[4] = 0x02;
    data2[5] = *pVL;
    data2[6] = *(pVL+1);
    data2[7] = *(pVL+2);
    data2[8] = *(pVL+3);
    data2[9] = 0xff;

    QByteArray data3; //电流档位
    data3.resize(10);
    data3[0] = 0x02;
    data3[1] = 0x00;
    data3[2] = SETCUGEARCMD;
    data3[3] = 0x00;
    data3[4] = 0x03;
    data3[5] = *pEG;
    data3[6] = *(pEG+1);
    data3[7] = *(pEG+2);
    data3[8] = *(pEG+3);
    data3[9] = 0xff;

    QByteArray data4; //电机细分
    data4.resize(10);
    data4[0] = 0x02;
    data4[1] = 0x00;
    data4[2] = SETCUGEARCMD;
    data4[3] = 0x00;
    data4[4] = 0x04;
    data4[5] = *pEL;
    data4[6] = *(pEL+1);
    data4[7] = *(pEL+2);
    data4[8] = *(pEL+3);
    data4[9] = 0xff;

    QByteArray data5; //编码逻辑方向
    data5.resize(10);
    data5[0] = 0x02;
    data5[1] = 0x00;
    data5[2] = SETCODELOGDI;
    data5[3] = 0x00;
    data5[4] = 0x05;
    data5[5] = *pCT;
    data5[6] = *(pCT+1);
    data5[7] = *(pCT+2);
    data5[8] = *(pCT+3);
    data5[9] = 0xff;

    QByteArray data6; //脉冲控制方式
    data6.resize(10);
    data6[0] = 0x02;
    data6[1] = 0x00;
    data6[2] = SETPLUSCTRLT;
    data6[3] = 0x00;
    data6[4] = 0x06;
    data6[5] = *pPT;
    data6[6] = *(pPT+1);
    data6[7] = *(pPT+2);
    data6[8] = *(pPT+3);
    data6[9] = 0xff;

    QByteArray data7; //负向最大允许位移
    data7.resize(10);
    data7[0] = 0x02;
    data7[1] = 0x00;
    data7[2] = SETNEGMAXPOS;
    data7[3] = 0x00;
    data7[4] = 0x07;
    data7[5] = *pMN;
    data7[6] = *(pMN+1);
    data7[7] = *(pMN+2);
    data7[8] = *(pMN+3);
    data7[9] = 0xff;

    QByteArray data8; //正向最大允许位移
    data8.resize(10);
    data8[0] = 0x02;
    data8[1] = 0x00;
    data8[2] = SETPOSMAXPOS;
    data8[3] = 0x00;
    data8[4] = 0x08;
    data8[5] = *pMP;
    data8[6] = *(pMP+1);
    data8[7] = *(pMP+2);
    data8[8] = *(pMP+3);
    data8[9] = 0xff;

    QByteArray data9; //减速时间
    data9.resize(10);
    data9[0] = 0x02;
    data9[1] = 0x00;
    data9[2] = SETDECDURCMD;
    data9[3] = 0x00;
    data9[4] = 0x09;
    data9[5] = *pDT;
    data9[6] = *(pDT+1);
    data9[7] = *(pDT+2);
    data9[8] = *(pDT+3);
    data9[9] = 0xff;

    QByteArray dataa; //加速时间
    dataa.resize(10);
    dataa[0] = 0x02;
    dataa[1] = 0x00;
    dataa[2] = SETACCDURCMD;
    dataa[3] = 0x00;
    dataa[4] = 0x0a;
    dataa[5] = *pAT;
    dataa[6] = *(pAT+1);
    dataa[7] = *(pAT+2);
    dataa[8] = *(pAT+3);
    dataa[9] = 0xff;

    QByteArray data = raw(convert4bytes(10000000), 4);

    qDebug() << data;
    //emit getData(data);
    //qDebug() << qa1;
}

void Form::on_pbPush_clicked()
{
    quint32 elecCtrl = ui->elecContrl->currentIndex(); //智能电流控制
    quint32 volLevel = ui->volLevel->currentIndex(); //参考电压
    quint32 elecGrade = ui->elecGrade->text().toUInt(); //电流档位
    quint32 elecLevel = ui->motorLevel->currentIndex(); //电机细分等级
    quint32 codeType = ui->codeLogicDirect->currentIndex(); //逻辑编码方向
    quint32 plusType = ui->plusType->currentIndex(); //脉冲控制方式
    quint32 maxN = ui->maximumNegative->text().toUInt(); //负向最大允许位移
    qDebug() << maxN;
    quint32 maxP = ui->maximumPositive->text().toUInt(); //正向最大允许位移
    quint32 decTime = ui->servoDecTime->text().toUInt(); //减速时间
    quint32 accTime = ui->servoAccTime->text().toUInt(); //加速时间

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
    quint8 head[] = {0x02,0x00,BATCHCONFCMD,0x00,BATCHCMDNUMB,0x00,0x00,0x00,0x00,0x00};
    //智能电流控制
    QByteArray qEC = raw(convert4bytes(elecCtrl), DATANUMBER);
    quint8 ec[] = {0x02,0x00,SETSMARTCURR,0x00,0x01,qEC[0],qEC[1],qEC[2],qEC[3],0x00};
    //参考电压
    QByteArray qVL = raw(convert4bytes(volLevel), DATANUMBER);
    quint8 vl[] = {0x02,0x00,SETREFVOLCMD,0x00,0x02,qVL[0],qVL[1],qVL[2],qVL[3],0x00};

    QByteArray qEG = raw(convert4bytes(elecGrade), DATANUMBER);
    quint8 eg[] = {0x02,0x00,SETCUGEARCMD,0x00,0x03,qEG[0],qEG[1],qEG[2],qEG[3],0x00};

    QByteArray qEL = raw(convert4bytes(elecLevel), DATANUMBER);
    quint8 el[] = {0x02,0x00,SETMOTDIVCMD,0x00,0x04,qEL[0],qEL[1],qEL[2],qEL[3],0x00};

    QByteArray qCT = raw(convert4bytes(codeType), DATANUMBER);
    quint8 ct[] = {0x02,0x00,SETCODELOGDI,0x00,0x05,qCT[0],qCT[1],qCT[2],qCT[3],0x00};

    QByteArray qPT = raw(convert4bytes(plusType), DATANUMBER);
    quint8 pt[] = {0x02,0x00,SETPLUSCTRLT,0x00,0x06,qPT[0],qPT[1],qPT[2],qPT[3],0x00};

    QByteArray qMN = raw(convert4bytes(maxN), DATANUMBER);
    quint8 mn[] = {0x02,0x00,SETNEGMAXPOS,0x00,0x07,qMN[0],qMN[1],qMN[2],qMN[3],0x00};

    QByteArray qMP = raw(convert4bytes(maxP), DATANUMBER);
    quint8 mp[] = {0x02,0x00,SETPOSMAXPOS,0x00,0x08,qMP[0],qMP[1],qMP[2],qMP[3],0x00};

    QByteArray qDT = raw(convert4bytes(decTime), DATANUMBER);
    quint8 dt[] = {0x02,0x00,SETDECDURCMD,0x00,0x09,qDT[0],qDT[1],qDT[2],qDT[3],0x00};

    QByteArray qAT = raw(convert4bytes(maxP), DATANUMBER);
    quint8 at[] = {0x02,0x00,SETACCDURCMD,0x00,0x0a,qAT[0],qAT[1],qAT[2],qAT[3],0x00};

    QByteArray qa1 = raw(head, BATCHCMDNUMB);
    QByteArray qa2 = raw(ec, BATCHCMDNUMB);
    QByteArray qa3 = raw(vl, BATCHCMDNUMB);
    QByteArray qa4 = raw(eg, BATCHCMDNUMB);
    QByteArray qa5 = raw(el, BATCHCMDNUMB);
    QByteArray qa6 = raw(ct, BATCHCMDNUMB);
    QByteArray qa7 = raw(pt, BATCHCMDNUMB);
    QByteArray qa8 = raw(mn, BATCHCMDNUMB);
    QByteArray qa9 = raw(mp, BATCHCMDNUMB);
    QByteArray qa10 = raw(dt, BATCHCMDNUMB);
    QByteArray qa11 = raw(at, BATCHCMDNUMB);

    QByteArray data;
    data.append(qa1).append(qa8);

    qDebug() << data.toHex().toUpper();
}

QByteArray Form::raw(quint8 *p, int size)
{
    QByteArray qa;
    for(int i=0; i<size; i++)
    {
        qa[i] = p[i];
    }

    return qa;
}

void Form::initConnect()
{
    connect(ui->emergencyButton, SIGNAL(clicked()), SLOT(emergencyStop()));
    //connect(ui->emergencyButton, SIGNAL(clicked()), SIGNAL(getData(QByteArray)));
}

quint8* Form::convert4bytes(quint32 n) {
    static quint8 temp[4];
    temp[0] = n;
    temp[1] = (n >> 8);
    temp[2] = (n >> 16);
    temp[3] = (n >> 24);
    /*
    temp[3] = (n >> 24);
    temp[2] = (n >> 16);
    temp[1] = (n >> 8);
    temp[0] = n;
    */

    return temp;
}

quint8* Form::convert2bytes(quint16 n) {
    static quint8 temp[2];
    temp[0] = (n >> 8) & 0xFF;
    temp[1] = n & 0xFF;

    return temp;
}

quint32 Form::power(int index)
{
    quint32 ret = 1;
    while(index--) {
        ret = ret * 2;
    }
    return ret;
}

QByteArray Form::convert(const Cmd &cmd)
{
    QByteArray qa;
    qa.append(cmd.id[0]).append(cmd.id[1]).append(cmd.master)
      .append(cmd.slave).append(cmd.data[0]).append(cmd.data[1])
      .append(cmd.data[2]).append(cmd.data[3]).append(cmd.check);
    //qDebug() << qa.toHex();
    return qa;
}

void Form::on_pbPull_clicked()
{
    QByteArray array = raw(convert4bytes(10000000), 4);
    qDebug() << array;
}
