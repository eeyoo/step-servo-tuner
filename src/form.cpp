#include "form.h"
#include "ui_form.h"

#include "command.h"
#include "absmovecmd.h"
#include <iostream>
#include <QMessageBox>

#include <QFileSystemModel>

#include <QtCore/QDebug>

Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);

    initUI();
    initConnect();
    ui->tableWidget->setAcceptDrops(false);

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

    ui->tableWidget = new QTableWidget(5,2,this);
    QTableWidgetItem *horzhead = new QTableWidgetItem();
    ui->tableWidget->setHorizontalHeaderItem(1, horzhead);

    for(int row=0; row < 5; row++) {
        for(int colum=0; colum < 3; colum++) {
            qDebug() << (row+1)*(colum+1);
            QTableWidgetItem *item = new QTableWidgetItem(tr("%1").arg((row+1)*(colum+1)));
            item->setText("h");
            ui->tableWidget->setItem(row, colum, item);
        }
    }

    QFileSystemModel *model = new QFileSystemModel;
    //ui->tableWidget->setModel(model);
}
