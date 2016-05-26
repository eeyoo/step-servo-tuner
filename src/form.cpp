#include "form.h"
#include "ui_form.h"

#include "command.h"
#include "configdialog.h"
#include <iostream>
#include <QMessageBox>

#include <QStandardItemModel>

#include <QtCore/QDebug>

Form::Form(QWidget *parent) :
    QWidget(parent),row(0),
    index(0), position(0),status(false),
    jmp_from(0), jmp_to(0),
    ui(new Ui::Form)
{
    ui->setupUi(this);
    config = new ConfigDialog;

    int level = config->configs().elecLevel;
    int circle = config->configs().circleLen;
    int div[] = {1,2,4,8,16,32,64,128,256};
    int deviceId = config->configs().deviceId;
    convert(id, deviceId, 2);
    param = 200 * div[level] / circle;
    //qDebug() << QString("param %1 pos %2").arg(param).arg(position);

    beta = 0.4 * div[level] / circle; // 200 * 100 / 50000

    int maxSpd = 100 / beta;
    ui->setRunSpd->setMaximum(maxSpd);
    //qDebug() << "param = " << param;
    initUI();
    initConnect();
    initModel();

    cmd_list = new QList<QByteArray>;
    spd_show(ui->setRunSpd->value());
}

Form::~Form()
{
    delete ui;
    delete model;
    delete config;
    delete cmd_list;
}

void Form::about()
{
    QMessageBox::about(this, tr("控制器配置程序"),
                       tr("/***********v1.0 测试版*************/"));
}

void Form::initUI()
{
    int countIn = config->configs().countIn;
    ui->jumpParam->setMaximum(countIn);
    ui->inputParam->setMaximum(countIn);

    int maxN = config->configs().maxN;
    int maxP = config->configs().maxP;
    ui->absMoveDistance->setMaximum(maxP);
    ui->absMoveDistance->setMinimum(maxN);
    ui->relMoveDistance->setMaximum(maxP);
    ui->relMoveDistance->setMinimum(maxN);
}

void Form::initConnect()
{
    //connect(ford_timer, SIGNAL(timeout()), this, SLOT(forward()));
    connect(ui->setRunSpd, SIGNAL(valueChanged(int)), this, SLOT(spd_show(int)));
}

void Form::initModel()
{
    model = new QStandardItemModel(0, 2, this);
    model->setHeaderData(0, Qt::Horizontal, tr("指令类型"));
    model->setHeaderData(1, Qt::Horizontal, tr("指令内容"));

    ui->tableView->setModel(model);
    QHeaderView *headerView = ui->tableView->horizontalHeader();
    headerView->setStretchLastSection(true);

}


void Form::receiveData(const QByteArray &data)
{
    echo = data;
    //qDebug() << "echo: " << echo.toHex();
}

void Form::dragEnterEvent(QDragEnterEvent *event)
{

}

void Form::dropEvent(QDropEvent *event)
{

}

void Form::on_absAddBtn_clicked()
{

    position = ui->absMoveDistance->value();

    moves.append(0);
    quint8 qpos[4];
    convert(qpos, position * param, 4);

    //qDebug() << QString("plus %1 pos %2").arg(position*param).arg(position);

    quint8 qPos[10] = {id[0],id[1],ABS_MOVE_CMD,0x01,0x00,qpos[0],qpos[1],qpos[2],qpos[3],0x00};

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
    int pos = ui->relMoveDistance->value();
    position += pos;
    moves.append(pos);

    quint8 qpos[4];
    convert(qpos, position * param, 4);

    //qDebug() << QString("plus %1 pos %2").arg(position*param).arg(position);

    quint8 qPos[10] = {id[0],id[1],ABS_MOVE_CMD,0x01,0x00,qpos[0],qpos[1],qpos[2],qpos[3],0x00};

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
    moves.append(0);

    int lpd = ui->setRunSpd->value(); //线速度mm/s - 转速 - 每秒脉冲数
    int spd = beta * lpd;
    //qDebug() << QString(tr("参数 %1 档位 %2%")).arg(beta).arg(spd);

    quint8 qspd[4];
    convert(qspd, spd, 4);

    quint8 qSpd[10] = {id[0],id[1],SETMOVESPCMD,0x01,0x00,qspd[0],qspd[1],qspd[2],qspd[3],0x00};
    QByteArray qa;
    array2qa(qa, qSpd, 10);
    cmd_list->append(qa);

    QStringList list;
    list << tr("设置速度指令") << QString(tr("线速度设置为 %1 mm/s")).arg(lpd);

    model->insertRow(row, QModelIndex());
    model->setData(model->index(row, 0), list.value(0));
    model->setData(model->index(row, 1), list.value(1));

    row++;

    ui->tableView->setModel(model);
}

void Form::on_resetBtn_clicked()
{
    cmd_list->clear();
    moves.clear();

    model->removeRows(0, row, QModelIndex());
    ui->tableView->setModel(model);
    row = 0;
    index = 0;
    position = 0;
}

void Form::on_deleteBtn_clicked()
{
    row--;
    index--;
    if (row == 0) {
        //row = 0;
        position = 0;
        index = 0;
    }

    if(row == jmp_from)
        jmp_to = 0;

    model->removeRow(row, QModelIndex());
    cmd_list->removeAt(row);
    position -= moves.at(row);
    moves.removeAt(row);

    //qDebug() << QString("position %1 row %2").arg(position).arg(row);
}


void Form::on_stepAct_clicked()
{
    //if(index == row)
    //    index = 0;

    if((index >= 0) && (index < row)) {
        emit sendData(cmd_list->at(index));
        ui->tableView->selectRow(index);
        index++;
    }

    if(index == jmp_from)
        index = jmp_to;

    //qDebug() << "row = " << row << " index = " << index;

}

void Form::spd_show(int lpd)
{
    int circle = config->configs().circleLen;
    //int lpd = ui->setRunSpd->value();
    double rps = (double)lpd / circle;
    ui->rpsLab->setText(QString(tr("转速：%1 rps(转每秒)")).arg(rps));
}

void Form::convert(quint8 *buf, int data, int size)
{
    for(int i=0; i<size; i++)
        buf[i] = data >> (8*i);
}

void Form::array2qa(QByteArray &data, quint8 *buf, int size)
{
    for(int i=0; i<size; i++)
        data[i] = buf[i];
}

void Form::on_stopAct_clicked()
{

    quint8 qStop[10] = {id[0],id[1],EMSTOP_CMD,0x01,0x00,0x00,0x00,0x00,0x00,0x00};
    QByteArray qa;
    array2qa(qa, qStop, 10);

    emit sendData(qa);
}

void Form::on_forwardAct_clicked()
{
    int len = cmd_list->count();
    quint8 qlen[4];
    convert(qlen, len, 4);

    quint8 qHead[10] = {id[0],id[1],CMDBATCHHEAD,0x01,0x00,qlen[0],qlen[1],qlen[2],qlen[3],0x00};
    QByteArray qa;
    array2qa(qa, qHead, 10);
    //cmd_list->prepend(qa);

    //QByteArray data;
    for(int i=0; i<len; i++)
        qa.append(cmd_list->at(i));

    //qDebug() << qa.toHex();
    emit sendData(qa);

}


void Form::on_opAddBtn_clicked()
{
    moves.append(0);
    int param = ui->opParam->value();
    int opType = ui->opType->currentIndex() + 1; //1-自增 2-自减
    quint8 pop[4];
    convert(pop,opType,4);
    quint8 op[] = {id[0],id[1],OPERATEPARAM,param,0x00,pop[0],pop[1],pop[2],pop[3],0x00};
    QByteArray qa;
    array2qa(qa, op, 10);
    cmd_list->append(qa);

    QStringList list;
    if(opType == 1)
        list << tr("操作参数指令") << QString(tr("编号 %1 参数 ++")).arg(param);
    else if(opType == 2)
        list << tr("操作参数指令") << QString(tr("编号 %1 参数 --")).arg(param);

    model->insertRow(row, QModelIndex());
    model->setData(model->index(row, 0), list.value(0));
    model->setData(model->index(row, 1), list.value(1));

    row++;

    ui->tableView->setModel(model);

}

void Form::on_jmpAddBtn_clicked()
{
    moves.append(0);
    int line = ui->jmpLine->value();

    if(line > row) {
        QMessageBox::warning(this, tr("警告"), QString(tr("跳转行不能超过 %1")).arg(row));
        return;
    }

    jmp_to = line-1;

    quint8 pln[2];
    convert(pln,line-1,2);
    quint8 ln[] = {id[0],id[1],JMP_CMD,0x00,0x00,0x00,0x00,pln[0],pln[1],0x00};
    QByteArray qa;
    array2qa(qa, ln, 10);
    cmd_list->append(qa);

    QStringList list;
    list << tr("无条件跳转指令") << QString(tr("无条件跳转至 %1行指令")).arg(line);

    model->insertRow(row, QModelIndex());
    model->setData(model->index(row, 0), list.value(0));
    model->setData(model->index(row, 1), list.value(1));

    row++;
    jmp_from = row;

    ui->tableView->setModel(model);
}

void Form::on_cmpAddBtn_clicked()
{
    moves.append(0);
    int line = ui->cmpLine->value();
    if(line > row) {
        QMessageBox::warning(this, tr("警告"), QString(tr("跳转行不能超过 %1")).arg(row));
        return;
    }

    int param = ui->cmpParam->value();
    int type = ui->cmpType->currentIndex() + 1;
    int value = ui->cmpVal->value();

    jmp_to = line-1;

    quint8 pln[2];
    convert(pln,line-1,2);
    quint8 val[2];
    convert(val, value, 2);
    quint8 ln[] = {id[0],id[1],CMP_CMD,param,type,val[0],val[1],pln[0],pln[1],0x00};
    QByteArray qa;
    array2qa(qa, ln, 10);
    cmd_list->append(qa);

    QStringList list;
    list << tr("有条件跳转指令");
    if(type == 1)
        list << QString(tr("编号 %1 参数大于 %2 跳转至 %3行指令")).arg(param).arg(value).arg(line);
    if(type == 2)
        list << QString(tr("编号 %1 参数等于 %2 跳转至 %3行指令")).arg(param).arg(value).arg(line);
    if(type == 3)
        list << QString(tr("编号 %1 参数小于 %2 跳转至 %3行指令")).arg(param).arg(value).arg(line);

    model->insertRow(row, QModelIndex());
    model->setData(model->index(row, 0), list.value(0));
    model->setData(model->index(row, 1), list.value(1));

    row++;
    jmp_from = row;

    ui->tableView->setModel(model);
}

void Form::on_jumpAddBtn_clicked()
{
    moves.append(0);
    int line = ui->jumpLine->value();
    if(line > row) {
        QMessageBox::warning(this, tr("警告"), QString(tr("跳转行不能超过 %1")).arg(row));
        return;
    }

    int param = ui->jumpParam->value();
    int state = ui->ioState->currentIndex();

    jmp_to = line-1;

    quint8 pln[2];
    convert(pln,line-1,2);
    quint8 ln[] = {id[0],id[1],IOJUMP_CMD,param,0x00,state,0x00,pln[0],pln[1],0x00};
    QByteArray qa;
    array2qa(qa, ln, 10);
    cmd_list->append(qa);

    QStringList list;
    list << tr("IO条件跳转指令");
    if(!state)
        list << QString(tr("编号 %1 IO端口高电平跳转至 %2行指令")).arg(param).arg(line);
    else
        list << QString(tr("编号 %1 IO端口低电平跳转至 %2行指令")).arg(param).arg(line);

    model->insertRow(row, QModelIndex());
    model->setData(model->index(row, 0), list.value(0));
    model->setData(model->index(row, 1), list.value(1));

    row++;
    jmp_from = row;

    ui->tableView->setModel(model);
}

void Form::on_inputAddBtn_clicked()
{
    moves.append(0);
    int param = ui->inputParam->value();
    int state = ui->inputState->currentIndex();

    quint8 cmd[] = {id[0],id[1],INPUT_CMD,param,0x00,state,0x00,0x00,0x00,0x00,0x00};
    QByteArray qa;
    array2qa(qa, cmd, 10);
    cmd_list->append(qa);

    QStringList list;
    list << tr("输入等待指令");
    if(!state)
        list << QString(tr("编号 %1 输入端口低电平等待")).arg(param);
    else
        list << QString(tr("编号 %1 输入端口高电平等待")).arg(param);

    model->insertRow(row, QModelIndex());
    model->setData(model->index(row, 0), list.value(0));
    model->setData(model->index(row, 1), list.value(1));

    row++;

    ui->tableView->setModel(model);
}

void Form::on_outputAddBtn_clicked()
{
    moves.append(0);
    int param = ui->outputParam->value();
    int state = ui->outputState->currentIndex();

    quint8 cmd[] = {id[0],id[1],SETOUT_CMD,param,0x00,state,0x00,0x00,0x00,0x00};
    QByteArray qa;
    array2qa(qa, cmd, 10);
    cmd_list->append(qa);

    QStringList list;
    list << tr("输出设置指令");
    if(!state)
        list << QString(tr("设置编号 %1 输出端口低电平")).arg(param);
    else
        list << QString(tr("设置编号 %1 输出端口高电平")).arg(param);

    model->insertRow(row, QModelIndex());
    model->setData(model->index(row, 0), list.value(0));
    model->setData(model->index(row, 1), list.value(1));

    row++;

    ui->tableView->setModel(model);
}

void Form::on_delayAddBtn_clicked()
{
    moves.append(0);
    int value = ui->delayVal->value();

    quint8 val[4];
    convert(val, value, 4);
    quint8 cmd[] = {id[0],id[1],DELAY_CMD,0x00,0x00,val[0],val[1],val[2],val[3],0x00};
    QByteArray qa;
    array2qa(qa, cmd, 10);
    cmd_list->append(qa);

    QStringList list;
    list << tr("延时等待指令") << QString(tr("延时等待 %1 毫秒")).arg(value);

    model->insertRow(row, QModelIndex());
    model->setData(model->index(row, 0), list.value(0));
    model->setData(model->index(row, 1), list.value(1));

    row++;

    ui->tableView->setModel(model);
}
