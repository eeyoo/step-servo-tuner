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
    index(0), position(0),status(false),
    ui(new Ui::Form)
{
    ui->setupUi(this);
    model = new QStandardItemModel();

    initUI();

    ford_timer = new QTimer(this);
    ford_timer->setInterval(1000); //间隔1秒

    initConnect();
    initModel();

    //m_list = new QStringList;
    cmd_list = new QList<QByteArray>;

}

Form::~Form()
{
    delete ui;
    delete ford_timer;
    delete model;
}

void Form::about()
{
    QMessageBox::about(this, tr("控制器配置程序"),
                       tr("/***********测试版*************/"));
}

void Form::initUI()
{

}

void Form::initConnect()
{
    connect(ford_timer, SIGNAL(timeout()), this, SLOT(forward()));
}

void Form::initModel()
{
    model = new QStandardItemModel(0, 2, this);
    model->setHeaderData(0, Qt::Horizontal, tr("指令类型"));
    model->setHeaderData(1, Qt::Horizontal, tr("指令内容"));
    ui->tableView->setModel(model);
}


void Form::receiveData(const QByteArray &data)
{
    echo = data;
}

void Form::dragEnterEvent(QDragEnterEvent *event)
{

}

void Form::dropEvent(QDropEvent *event)
{

}

void Form::on_absAddBtn_clicked()
{

    position = ui->absMoveDistance->text().toInt();

    quint8 qpos[4];
    convert(qpos, position);

    quint8 qPos[10] = {0x02,0x00,ABS_MOVE_CMD,0x01,0x00,qpos[0],qpos[1],qpos[2],qpos[3],0x00};
    //quint8 qSpd[10] = {0x02,0x00,SETMOVESPCMD,0x01,0x00,qspd[0],qspd[1],qspd[2],qspd[3],0x00};

    QByteArray qa;
    array2qa(qa, qPos, 10);
    //qa.append(qa1).append(qa2);
    //qDebug() << qa.toHex();
    //m_list->append(qa1.append(qa2).toHex());
    //m_list->append(qa2.toHex());
    cmd_list->append(qa);

    QStringList list;
    list << tr("绝对运动指令") << QString(tr("绝对运行距离至 %1mm")).arg(position);


    model->insertRow(row, QModelIndex());
    model->setData(model->index(row, 0), list.value(0));
    model->setData(model->index(row, 1), list.value(1));

    row++;

    ui->tableView->setModel(model);
}

void Form::on_relaAddBtn_clicked()
{
    int pos = ui->relMoveDistance->text().toInt();
    position += pos;

    quint8 qpos[4];
    convert(qpos, position);
    //quint8 qspd[4];
    //convert(qspd, spd);

    quint8 qPos[10] = {0x02,0x00,ABS_MOVE_CMD,0x01,0x00,qpos[0],qpos[1],qpos[2],qpos[3],0x00};
    //quint8 qSpd[10] = {0x02,0x00,SETMOVESPCMD,0x01,0x00,qspd[0],qspd[1],qspd[2],qspd[3],0x00};
    QByteArray qa;
    array2qa(qa, qPos, 10);
    //qa.append(qa1).append(qa2);
    //qDebug() << qa.toHex();
    //m_list->append(qa1.append(qa2).toHex());
    //m_list->append(qa2.toHex());
    cmd_list->append(qa);

    QStringList list;
    list << tr("相对运动指令") << QString(tr("相对运行距离 %1mm")).arg(pos);

    model->insertRow(row, QModelIndex());
    model->setData(model->index(row, 0), list.value(0));
    model->setData(model->index(row, 1), list.value(1));

    row++;

    ui->tableView->setModel(model);
}

void Form::on_setSpdBtn_clicked()
{
    int spd = ui->setRunSpd->value();
    quint8 qspd[4];
    convert(qspd, spd);

    quint8 qSpd[10] = {0x02,0x00,SETMOVESPCMD,0x01,0x00,qspd[0],qspd[1],qspd[2],qspd[3],0x00};
    QByteArray qa;
    array2qa(qa, qSpd, 10);
    cmd_list->append(qa);

    QStringList list;
    list << tr("设置速度指令") << QString(tr("设置速度档位 %1%")).arg(spd);

    model->insertRow(row, QModelIndex());
    model->setData(model->index(row, 0), list.value(0));
    model->setData(model->index(row, 1), list.value(1));

    row++;

    ui->tableView->setModel(model);
}

void Form::on_resetBtn_clicked()
{
    cmd_list->clear();

    model->removeRows(0, row, QModelIndex());
    ui->tableView->setModel(model);
    row = 0;
    index = 0;
}

void Form::on_deleteBtn_clicked()
{
    row--;
    index--;
    if (row < 0) {
        row = 0;
    }


    model->removeRow(row, QModelIndex());
    cmd_list->removeAt(row);


}


void Form::on_stepAct_clicked()
{
    qDebug() << "row = " << row << " index = " << index;

    if(index > row-1)
        index = 0;

    if((index >= 0) && (index < row)) {
        emit sendData(cmd_list->at(index));
        ui->tableView->selectRow(index);
        index++;
    }


}

void Form::convert(quint8 *buf, int data)
{
    buf[0] = data;
    buf[1] = data >> 8;
    buf[2] = data >> 16;
    buf[3] = data >> 24;
}

void Form::array2qa(QByteArray &data, quint8 *buf, int size)
{
    for(int i=0; i<size; i++)
        data[i] = buf[i];
}

void Form::on_stopAct_clicked()
{

    ford_timer->stop();
    status = false;

    quint8 qStop[10] = {0x02,0x00,EMSTOP_CMD,0x01,0x00,0x00,0x00,0x00,0x00,0x00};
    QByteArray qa;
    array2qa(qa, qStop, 10);
    //cmd_list->append(qa);
    emit sendData(qa);
}

void Form::forward()
{
    if(index > row-1) {
        index = row-1;
        ford_timer->stop();
        status = false;
    } else if(index < 0) {
        index = 0;
    }
    qDebug() << "row = " << row << " index = " << index;

    emit sendData(cmd_list->at(index));

    ui->tableView->selectRow(index);
    index++;
}

void Form::on_forwardAct_clicked()
{
    /*
    index = 0;
    ford_timer->start();
    status = true;
    */
    int len = cmd_list->count();
    quint8 qlen[4];
    convert(qlen, len);

    quint8 qHead[10] = {0x02,0x00,CMDBATCHHEAD,0x01,0x00,qlen[0],qlen[1],qlen[2],qlen[3],0x00};
    QByteArray qa;
    array2qa(qa, qHead, 10);
    //cmd_list->prepend(qa);

    //QByteArray data;
    for(int i=0; i<len; i++)
        qa.append(cmd_list->at(i));

    //qDebug() << qa.toHex();
    emit sendData(qa);

}

