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

    ford_timer = new QTimer(this);
    ford_timer->setInterval(1000); //间隔1秒

    back_timer = new QTimer(this);
    back_timer->setInterval(1000); //间隔1秒

    initConnect();
    initModel();

    //m_list = new QStringList;
    cmd_list = new QList<QByteArray>;

}

Form::~Form()
{
    delete ui;
    delete ford_timer;
    delete back_timer;
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
    connect(back_timer, SIGNAL(timeout()), this, SLOT(backward()));
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

    int pos = ui->absMoveDistance->text().toInt();
    int spd = ui->absRunSpd->value();

    quint8 qpos[4];
    convert(qpos, pos);
    quint8 qspd[4];
    convert(qspd, spd);

    //QByteArray qa;
    quint8 qPos[10] = {0x02,0x00,ABS_MOVE_CMD,0x01,0x00,qpos[0],qpos[1],qpos[2],qpos[3],0x00};
    quint8 qSpd[10] = {0x02,0x00,SETMOVESPCMD,0x01,0x00,qspd[0],qspd[1],qspd[2],qspd[3],0x00};

    QByteArray qa1, qa2;
    array2qa(qa1, qPos, 10);
    array2qa(qa2, qSpd, 10);
    //qa.append(qa1).append(qa2);
    //qDebug() << qa.toHex();
    //m_list->append(qa1.append(qa2).toHex());
    //m_list->append(qa2.toHex());
    cmd_list->append(qa1.append(qa2));

    QStringList list;
    list << tr("绝对运动指令") << QString(tr("绝对运行距离 %1mm， 运行速度档位 %2\%")).arg(pos).arg(spd);


    model->insertRow(row, QModelIndex());
    model->setData(model->index(row, 0), list.value(0));
    model->setData(model->index(row, 1), list.value(1));

    row++;

    ui->tableView->setModel(model);
}

void Form::on_relaAddBtn_clicked()
{
    int pos = ui->relMoveDistance->text().toInt();
    int spd = ui->relRunSpd->value();

    quint8 qpos[4];
    convert(qpos, pos);
    quint8 qspd[4];
    convert(qspd, spd);

    //QByteArray qa;
    quint8 qPos[10] = {0x02,0x00,REL_MOVE_CMD,0x01,0x00,qpos[0],qpos[1],qpos[2],qpos[3],0x00};
    quint8 qSpd[10] = {0x02,0x00,SETMOVESPCMD,0x01,0x00,qspd[0],qspd[1],qspd[2],qspd[3],0x00};
    QByteArray qa1, qa2;
    array2qa(qa1, qPos, 10);
    array2qa(qa2, qSpd, 10);
    //qa.append(qa1).append(qa2);
    //qDebug() << qa.toHex();
    //m_list->append(qa1.append(qa2).toHex());
    //m_list->append(qa2.toHex());
    cmd_list->append(qa1.append(qa2));

    QStringList list;
    list << tr("相对运动指令") << QString(tr("相对运行距离 %1mm， 运行速度档位 %2\%")).arg(pos).arg(spd);

    model->insertRow(row, QModelIndex());
    model->setData(model->index(row, 0), list.value(0));
    model->setData(model->index(row, 1), list.value(1));

    row++;

    ui->tableView->setModel(model);
}

void Form::on_resetBtn_clicked()
{
    //m_list->clear(); //清空指令序列
    cmd_list->clear();

    model->removeRows(0, row, QModelIndex());
    ui->tableView->setModel(model);
    row = 0;
    index = 0;
}

void Form::on_deleteBtn_clicked()
{
    if (row < 0)
        row = 0;

    model->removeRow(row, QModelIndex());

    //m_list->removeAt(row); //删除追加指令
    cmd_list->removeAt(row);

    row--;
}


void Form::on_stepAct_clicked()
{
    if (index > row-1)
        index = row-1;
    qDebug() << "row = " << row << " index = " << index;

    emit sendData(cmd_list->at(index));

    //qDebug() << m_list->value(index);
    ui->tableView->selectRow(index);
    index++;
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
    back_timer->stop();
    status = false;
}

void Form::run()
{

}

void Form::backward()
{
    if(index < 0) {
        index = 0;
        back_timer->stop();
        status = false;
    } else if(index > row-1) {
        index = row-1;
    }
    qDebug() << "row = " << row << " index = " << index;

    emit sendData(cmd_list->at(index));

    ui->tableView->selectRow(index);
    //qDebug() << m_list->value(index);
    index--;
}

void Form::on_backwardAct_clicked()
{
    back_timer->start();
    status = true;
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
    //qDebug() << m_list->value(index);
    index++;
}

void Form::on_forwardAct_clicked()
{

    ford_timer->start();
    status = true;
}
