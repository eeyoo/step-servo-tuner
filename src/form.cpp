#include "form.h"
#include "ui_form.h"

#include "command.h"
#include "absmovecmd.h"
#include <iostream>
#include <QMessageBox>

#include <QFileSystemModel>
#include <QStandardItemModel>

#include <QTimer>

#include <QtCore/QDebug>

Form::Form(QWidget *parent) :
    QWidget(parent),row(0),
    index(0), status(false),
    ui(new Ui::Form)
{
    ui->setupUi(this);
    model = new QStandardItemModel();

    initUI();

    timer = new QTimer(this);
    timer->setInterval(1000);

    initConnect();
    initModel();

    m_list = new QStringList;

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

}

void Form::initConnect()
{
    //connect(ui->emergencyButton, SIGNAL(clicked()), SLOT(emergencyStop()));
    //connect(ui->emergencyButton, SIGNAL(clicked()), SIGNAL(getData(QByteArray)));
    connect(timer, SIGNAL(timeout()), this, SLOT(step()));
}

void Form::initModel()
{
    model = new QStandardItemModel(0, 2, this);
    model->setHeaderData(0, Qt::Horizontal, tr("指令类型"));
    model->setHeaderData(1, Qt::Horizontal, tr("指令内容"));

}


void Form::receiveData(const QByteArray &data)
{
    echo = data;
    qDebug() << echo.toHex().toUpper();
}

void Form::dragEnterEvent(QDragEnterEvent *event)
{

}

void Form::dropEvent(QDropEvent *event)
{

}

void Form::on_absAddBtn_clicked()
{

    int pos = ui->absMoveDistance->text().toInt();
    int spd = ui->absRunSpd->value();

    quint8 *qpos = convert(pos);
    quint8 *qspd = convert(spd);
    QByteArray qa;
    quint8 qPos[10] = {0x02,0x00,ABS_MOVE_CMD,0x01,0x00,qpos[0],qpos[1],qpos[2],qpos[3],0x00};
    quint8 qSpd[10] = {0x02,0x00,SETMOVESPCMD,0x01,0x00,qspd[0],qspd[1],qspd[2],qspd[3],0x00};

    QByteArray qa1 = array(qPos, 4);
    QByteArray qa2 = array(qSpd, 4);
    qa.append(qa1).append(qa2);
    qDebug() << qa.toHex();
    m_list->append(qa.toHex());

    QStringList list;
    list << tr("绝对运动指令") << QString(tr("绝对运行距离 %1mm， 运行速度档位 %2\%")).arg(pos).arg(spd);
    //m_list->append(tr("绝对运行指令+目标速度设置指令"));


    model->insertRow(row, QModelIndex());
    model->setData(model->index(row, 0), list.value(0));
    model->setData(model->index(row, 1), list.value(1));

    row++;

    ui->tableView->setModel(model);
    //qDebug() << "row = " + row;
}

void Form::on_relaAddBtn_clicked()
{
    int pos = ui->relMoveDistance->text().toInt();
    int spd = ui->relRunSpd->value();

    quint8 *qpos = convert(pos);
    quint8 *qspd = convert(spd);
    QByteArray qa;
    quint8 qPos[10] = {0x02,0x00,REL_MOVE_CMD,0x01,0x00,qpos[0],qpos[1],qpos[2],qpos[3],0x00};
    quint8 qSpd[10] = {0x02,0x00,SETMOVESPCMD,0x01,0x00,qspd[0],qspd[1],qspd[2],qspd[3],0x00};
    QByteArray qa1 = array(qPos, 4);
    QByteArray qa2 = array(qSpd, 4);
    qa.append(qa1).append(qa2);
    qDebug() << qa.toHex();
    m_list->append(qa.toHex());

    QStringList list;
    list << tr("相对运动指令") << QString(tr("相对运行距离 %1mm， 运行速度档位 %2\%")).arg(pos).arg(spd);
    //m_list->append(tr("相对运行指令+目标速度设置指令"));

    model->insertRow(row, QModelIndex());
    model->setData(model->index(row, 0), list.value(0));
    model->setData(model->index(row, 1), list.value(1));

    row++;

    ui->tableView->setModel(model);
    //qDebug() << "row = " + row;
}

void Form::on_resetBtn_clicked()
{
    m_list->clear(); //清空指令序列

    model->removeRows(0, row, QModelIndex());
    ui->tableView->setModel(model);
    row = 0;
    index = 0;
    //qDebug() << "row = " + row;
}

void Form::on_autoAct_clicked()
{
    //指令自动执行
    //指令是打包发，还是一条一条发
    //指令model需要改写
    //增加定时器触发事件
    //增加串口数据发送和接收事件

    timer->start();
    status = true;

}

void Form::on_deleteBtn_clicked()
{
    row--;
    model->removeRow(row, QModelIndex());

    m_list->removeAt(row); //删除追加指令
    if (row < 0)
        row = 0;
    //qDebug() << "row = " << row;
}


void Form::on_stepAct_clicked()
{
    if (index == row)
        index = 0;

    qDebug() << m_list->value(index);
    ui->tableView->selectRow(index);
    index++;


}

quint8* Form::convert(int data)
{
    static quint8 temp[4];
    //for(int i=0; i<size; i++)
    temp[0] = data;
    temp[1] = data >> 8;
    temp[2] = data >> 16;
    temp[3] = data >> 24;

    return temp;
}

QByteArray Form::array(quint8 *data, int size)
{
    QByteArray array;
    for(int i=0; i<size; i++)
        array[i] = data[i];

    return array;
}

void Form::step()
{
    if (index == row)
        index = 0;
    ui->tableView->selectRow(index);
    qDebug() << m_list->value(index);
    index++;
}

void Form::on_stopAct_clicked()
{
    timer->stop();
    status = false;
}
