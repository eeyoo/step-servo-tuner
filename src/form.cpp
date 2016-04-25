#include "form.h"
#include "ui_form.h"

#include "command.h"
#include "absmovecmd.h"
#include <iostream>
#include <QMessageBox>

#include <QFileSystemModel>
#include <QStandardItemModel>

#include <QtCore/QDebug>

Form::Form(QWidget *parent) :
    QWidget(parent),row(0),
    ui(new Ui::Form)
{
    ui->setupUi(this);
    model = new QStandardItemModel();

    initUI();
    initConnect();
    //ui->tableWidget->setAcceptDrops(false);

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
    QStringList header;
    header << tr("指令名称") << tr("指令内容");
    model->setHorizontalHeaderLabels(header);
    ui->tableView->setModel(model);
}

void Form::initConnect()
{
    //connect(ui->emergencyButton, SIGNAL(clicked()), SLOT(emergencyStop()));
    //connect(ui->emergencyButton, SIGNAL(clicked()), SIGNAL(getData(QByteArray)));
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
    //tableWidget增加一行
    //指令流增加两条指令（绝对位移，目标速度）
//    QTableWidgetItem *newItem = new QTableWidgetItem();
//    newItem->setIcon(QIcon(QPixmap(":/images/clear.png")));
//    ui->tableWidget->setItem(1,1,newItem);


    //ui->tableWidget->setHorizontalHeaderLabels(header);
    //ui->tableWidget->setItem(0,0, new QTableWidgetItem("John"));
    //ui->tableWidget->setItem(1,0, new QTableWidgetItem("Mars"));
    //ui->tableWidget->setItem(2,0, new QTableWidgetItem("Hero"));

    //ui->tableWidget->setModel(model);
    //model->setHorizontalHeaderLabels(header);

    quint32 pos = ui->absMoveDistance->text().toUInt();
    quint32 spd = ui->absRunSpd->value();

    QStringList list;
    list << tr("绝对运动指令") << QString(tr("绝对运行距离 %1mm， 运行速度档位 %2\%")).arg(pos).arg(spd);
    QStandardItem *item = new QStandardItem(list.at(0));
    model->setItem(row,0, item);

    QStandardItem *item1 = new QStandardItem(list.at(1));
    //item->setText(QString::number(spd));
    model->setItem(row,1,item1);

    row++;

    ui->tableView->setModel(model);
}

void Form::on_relaAddBtn_clicked()
{
    int pos = ui->relMoveDistance->text().toInt();
    quint32 spd = ui->relRunSpd->value();

    QStringList list;
    list << tr("相对运动指令") << QString(tr("相对运行距离 %1mm， 运行速度档位 %2\%")).arg(pos).arg(spd);
    //QStandardItem *item = new QStandardItem(list.at(0));
    //model->setItem(row,0, item);
    //QStandardItem *item1 = new QStandardItem(list.at(1));
    //model->setItem(row,1,item1);
    QList<QStandardItem*> items;
    //items->append(new QStandardItem(list.at(0)));
    //items->append(new QStandardItem(list.at(1)));
    items << new QStandardItem(list.at(0)) << new QStandardItem(list.at(1));
    model->appendRow(items);

    row++;

    ui->tableView->setModel(model);

}

void Form::on_resetBtn_clicked()
{
    row = 0;
    model->clear();
    QStringList header;
    header << tr("指令名称") << tr("指令内容");
    model->setHorizontalHeaderLabels(header);
    ui->tableView->setModel(model);
}

void Form::on_autoAct_clicked()
{
    //指令自动执行
    //指令是打包发，还是一条一条发
    //指令model需要改写
    //增加定时器触发事件
    //增加串口数据发送和接收事件

}

void Form::on_deleteBtn_clicked()
{
    //ui->tableView->selectRow();
    //删除选中行
    if (!row--)
        row = 0;
    model->removeRow(row);
    qDebug() << "row value is " << row;
}
