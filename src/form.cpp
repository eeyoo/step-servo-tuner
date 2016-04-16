#include "form.h"
#include "ui_form.h"

#include "command.h"
#include "absmovecmd.h"
#include <iostream>
#include <QMessageBox>

#include <QtCore/QDebug>

Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);

    initUI();
    initConnect();
}

Form::~Form()
{
    delete ui;
}

void Form::about()
{
    QMessageBox::about(this, tr("控制器配置程序"),
                       tr("/***********开发版 开发者@吴飞林*************/"));
}

void Form::initUI()
{
    ui->backHomeDirection->addItem(QWidget::tr("正向"));
    ui->backHomeDirection->addItem(QWidget::tr("反向"));

    ui->backHomeGear->addItem(QWidget::tr("快档"));
    ui->backHomeGear->addItem(QWidget::tr("慢档"));
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
    //connect(ui->emergencyButton, SIGNAL(clicked()), SLOT(emergencyStop()));
    //connect(ui->emergencyButton, SIGNAL(clicked()), SIGNAL(getData(QByteArray)));
}

quint8* Form::convert4bytes(quint32 n) {
    static quint8 temp[4];
    temp[0] = n;
    temp[1] = (n >> 8);
    temp[2] = (n >> 16);
    temp[3] = (n >> 24);

    return temp;
}

quint8* Form::convert2bytes(quint32 n) {
    static quint8 temp[2];
    temp[0] = n;
    temp[1] = n >> 8;

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

void Form::on_absoluteMove_clicked()
{
    //绝对运行距离
    quint32 distance = ui->distance->text().toUInt();
    quint8 *data = convert4bytes(distance);
    quint8 *id = convert2bytes(EQUIPID);
    quint8 ds[] = {id[0],id[1],ABS_MOVE_CMD,0x00,0x00,data[0],data[1],data[2],data[3],0x00};
    QByteArray qa = raw(ds, BATCHCMDNUMB);
    qDebug() << qa.toHex();
    emit sendData(qa);
}

void Form::on_relativeMove_clicked()
{
    //相对运行距离
    quint32 distance = ui->distance->text().toUInt();
    quint8 *data = convert4bytes(distance);
    quint8 *id = convert2bytes(EQUIPID);
    quint8 ds[] = {id[0],id[1],REL_MOVE_CMD,0x00,0x00,data[0],data[1],data[2],data[3],0x00};
    QByteArray qa = raw(ds, BATCHCMDNUMB);
    qDebug() << qa.toHex();
    emit sendData(qa);
    //成员变量 echo接收控制器返回数据
    //返回10个字节数据 6至9字节为返回值

}

void Form::on_stopMove_clicked()
{
    //停止 -> 设置目标速度为0
    quint8 *id = convert2bytes(EQUIPID);
    quint8 cmd[] = {id[0],id[1],SETMOVESPCMD,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
    QByteArray qa = raw(cmd, BATCHCMDNUMB);
    qDebug() << qa.toHex();
    emit sendData(qa);
}

void Form::receiveData(const QByteArray &data)
{
    echo = data;
    qDebug() << echo.toHex().toUpper();
}
