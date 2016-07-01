#include "form.h"
#include "ui_form.h"

#include "command.h"
#include "configdialog.h"

#include <QMessageBox>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>

#include <QStandardItemModel>
#include <QMouseEvent>

#include <QtCore/QDebug>

Form::Form(QWidget *parent) :
    QWidget(parent),
    index(0),
    select_line(-1),
    mLine(-1),
    ui(new Ui::Form)
{
    ui->setupUi(this);
    config = new ConfigDialog;
    itemList = new CommandItemList(this);

    int level = config->configs().elecLevel;
    int circle = config->configs().circleLen;
    int div[] = {1,2,4,8,16,32,64,128,256};
    deviceId = config->configs().deviceId;

    int alpha = 200 * div[level] / circle;
    double beta = 0.4 * div[level] / circle; // 系数 200*100/50000

    int maxSpd = 100 / beta;
    ui->setRunSpd->setMaximum(maxSpd);

    cmd = new Command(alpha, beta);

    initUI();
    initConnect();
    initModel();

    spd_show(ui->setRunSpd->value());

    ui->parentToolBox->setCurrentIndex(0);
    ui->moveToolBox->setCurrentIndex(0);

    cmdType = -1;
    op = APP;
    quit = false;
}

Form::~Form()
{
    delete ui;
    delete cmd;
    delete config;
    delete itemList;
}

void Form::about()
{
    QMessageBox::about(this, tr("控制器应用程序"),tr("V1.0 版应用程序"));
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
    connect(ui->tableView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(tableDoubleClick(QModelIndex)));
    connect(ui->tableView, SIGNAL(clicked(QModelIndex)), this, SLOT(tableClick(QModelIndex)));
    connect(ui->tableView, SIGNAL(clicked(QModelIndex)), this, SLOT(showToolBox(QModelIndex)));
}

void Form::initModel()
{
    /*
    model = new QStandardItemModel(0, 2, this);
    model->setHeaderData(0, Qt::Horizontal, tr("指令类型"));
    model->setHeaderData(1, Qt::Horizontal, tr("指令内容"));
    */

    QHeaderView *headerView = ui->tableView->horizontalHeader();
    headerView->setStretchLastSection(true);

}

void Form::dragEnterEvent(QDragEnterEvent *event)
{

}

void Form::dropEvent(QDropEvent *event)
{

}

bool Form::saveProgFile(QString fileName) const
{
    return itemList->save(fileName);
}

bool Form::loadProgFile(QString fileName)
{
    return itemList->load(fileName);
}

void Form::operate(Command &cmd, QStringList &list)
{
    switch (op) {
    case APP: //默认追加
        //qDebug() << "========= APP ===========";
        //指令序列 - 模型
        itemList->append(cmd, list);
        break;
    case EDIT://指令修改
        //qDebug() << "========= EDIT ===========";
        break;
    case INSE://指令插入 默认前查
        //qDebug() << "========= INSE ===========";
        itemList->insert(cmd, list, mLine);
        mLine++;//选中行递增
        break;
    default:
        break;
    }

    ui->tableView->setModel(itemList->pmodel());
}

void Form::on_absAddBtn_clicked()
{

    int position = ui->absMoveDistance->value();

    int params[2] = {deviceId, position};
    Command acmd(params, Command::ABS);
    //acmd.pp();
    qDebug() << acmd.data().toHex();

    QStringList list;
    list << tr("绝对运动指令") << QString(tr("绝对运行距离至 %1mm")).arg(position);


    operate(acmd, list);
}

void Form::on_relaAddBtn_clicked()
{
    int pos = ui->relMoveDistance->value();
    int base = itemList->pos();//获取指令序列当前绝对位置

    //moves.append(pos);

    //int val = position*param;
    int params[3] = {deviceId, pos, base}; //绝对位置 = 增量 + 基准
    Command acmd(params, Command::RELA);
    qDebug() << acmd.data().toHex();

    QStringList list;
    list << tr("相对运动指令") << QString(tr("相对运行距离 %1mm")).arg(pos);

    operate(acmd, list);
}

void Form::on_setSpdBtn_clicked()
{
    //moves.append(0);

    int lpd = ui->setRunSpd->value(); //线速度mm/s - 转速 - 每秒脉冲数
    //qDebug() << QString(tr("参数 %1 档位 %2%")).arg(beta).arg(spd);

    int params[2] = {deviceId, lpd};
    Command acmd(params, Command::SPD);
    qDebug() << acmd.data().toHex();

    QStringList list;
    list << tr("设置速度指令") << QString(tr("线速度设置为 %1 mm/s")).arg(lpd);

    operate(acmd, list);
}

void Form::on_delayAddBtn_clicked()
{
    //moves.append(0);
    int value = ui->delayVal->value();

    int params[2] = {deviceId, value};
    Command acmd(params, Command::DELAY);
    qDebug() << acmd.data().toHex();

    QStringList list;
    list << tr("延时等待指令") << QString(tr("延时等待 %1 毫秒")).arg(value);

    operate(acmd, list);
}
/*
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
*/
void Form::spd_show(int lpd)
{
    int circle = config->configs().circleLen;
    //int lpd = ui->setRunSpd->value();
    double rps = (double)lpd / circle;
    ui->rpsLab->setText(QString(tr("转速：%1 rps(转每秒)")).arg(rps));
}

void Form::on_stopAct_clicked()
{
    int params[2] = {deviceId, 0};
    Command stopcmd(params, Command::STOP);

    qDebug() << stopcmd.data().toHex();
    emit sendData(stopcmd.data());
}

void Form::on_forwardAct_clicked()
{
    int len = itemList->size();

    int params[2] = {deviceId, len};
    Command acmd(params, Command::HEAD);
    qDebug() << acmd.data().toHex();

    QByteArray qa = acmd.data();
    itemList->output(qa);
    qDebug() << qa.toHex();

    //qDebug() << acmd.data().append(qa).toHex();
    //emit sendData(qa);
}

void Form::on_opAddBtn_clicked()
{
    //moves.append(0);
    int param = ui->opParam->value();
    int opType = ui->opType->currentIndex(); //1-自增 2-自减

    int params[3] = {deviceId, opType, param};
    Command acmd(params, Command::OPER);
    qDebug() << acmd.data().toHex();

    QStringList list;
    if(opType == 0)
        list << tr("操作参数指令") << QString(tr("编号 %1 参数 ++")).arg(param);
    else if(opType == 1)
        list << tr("操作参数指令") << QString(tr("编号 %1 参数 --")).arg(param);

    operate(acmd, list);
}

void Form::on_jmpAddBtn_clicked()
{
    //moves.append(0);
    int line = ui->jmpLine->value();

    int rows = itemList->size();
    //qDebug() << tr("无条件跳转至 %1 行，总行数 %2").arg(line).arg(rows);

    if(line > rows) {
        QMessageBox::warning(this, tr("警告"), QString(tr("跳转行不能超过 %1 行")).arg(rows));
        return;
    }

    int params[4] = {deviceId, 0, line, 0};
    Command acmd(params, Command::JMP);
    qDebug() << acmd.data().toHex();

    QStringList list;
    list << tr("无条件跳转指令") << QString(tr("无条件跳转至 %1 行指令")).arg(line);

    operate(acmd, list);
}

void Form::on_cmpAddBtn_clicked()
{
    //moves.append(0);
    int line = ui->cmpLine->value();
    int rows = itemList->size();

    if(line > rows) {
        QMessageBox::warning(this, tr("警告"), QString(tr("跳转行不能超过 %1 行")).arg(rows));
        return;
    }

    int param = ui->cmpParam->value();
    int type = ui->cmpType->currentIndex();
    int value = ui->cmpVal->value();

    int params[5] = {deviceId, value, line, param, type};
    Command acmd(params, Command::CMP);
    qDebug() << acmd.data().toHex();

    QStringList list;
    list << tr("有条件跳转指令");
    if(type == 0)
        list << QString(tr("编号 %1 参数大于 %2 跳转至 %3 行指令")).arg(param).arg(value).arg(line);
    if(type == 1)
        list << QString(tr("编号 %1 参数等于 %2 跳转至 %3 行指令")).arg(param).arg(value).arg(line);
    if(type == 2)
        list << QString(tr("编号 %1 参数小于 %2 跳转至 %3 行指令")).arg(param).arg(value).arg(line);

    operate(acmd, list);
}

void Form::on_jumpAddBtn_clicked()
{
    //moves.append(0);
    int line = ui->jumpLine->value();
    int rows = itemList->size();
    if(line > rows) {
        QMessageBox::warning(this, tr("警告"), QString(tr("跳转行不能超过 %1 行")).arg(rows));
        return;
    }

    int param = ui->jumpParam->value();
    int state = ui->ioState->currentIndex();//0-低电平 1-高电平

    //jmp_to = line-1;

    int params[4] = {deviceId, state, line, param};
    Command acmd(params, Command::IOJMP);
    qDebug() << acmd.data().toHex();

    QStringList list;
    list << tr("IO条件跳转指令");
    if(!state)
        list << QString(tr("编号 %1 IO端口高电平跳转至 %2 行指令")).arg(param).arg(line);
    else
        list << QString(tr("编号 %1 IO端口低电平跳转至 %2 行指令")).arg(param).arg(line);

    operate(acmd, list);
}

void Form::on_inputAddBtn_clicked() //输入等待
{
    //moves.append(0);
    int param = ui->inputParam->value();
    int state = ui->inputState->currentIndex();

    int params[4] = {deviceId, state, 0, param};
    Command acmd(params, Command::INPUT);
    qDebug() << acmd.data().toHex();

    QStringList list;
    list << tr("输入等待指令");
    if(!state)
        list << QString(tr("编号 %1 输入端口低电平等待")).arg(param);
    else
        list << QString(tr("编号 %1 输入端口高电平等待")).arg(param);

    operate(acmd, list);
}

void Form::on_outputAddBtn_clicked() //输出主动
{
    //moves.append(0);
    int param = ui->outputParam->value();
    int state = ui->outputState->currentIndex();

    int params[4] = {deviceId, state, 0, param};
    Command acmd(params, Command::SETOUT);
    qDebug() << acmd.data().toHex();

    QStringList list;
    list << tr("输出设置指令");
    if(!state)
        list << QString(tr("设置编号 %1 输出端口断开")).arg(param);
    else
        list << QString(tr("设置编号 %1 输出端口接通")).arg(param);

    operate(acmd, list);
}

void Form::tableDoubleClick(const QModelIndex &/*index*/)
{
    // 具体到某一行数据被双击
    // 需自定义一种模型，便于识别末行指令的类型
    // 然后展开相应的指令编辑区，用户修改指令
    // 数据模型还需要根据修改参数更新显示文本和指令
    //int nr = index.row();
    //qDebug() << tr("%1 row double clicked. data %2").arg(nr).arg(index.data().toString());

    //确定行 确定指令类型
    // 加入指令按钮有两种状态 新增和修改
    // 首次是新增 本方法改变状态为修改
    // 行确定修改模型
    // 类型确定修改指定模型行的数据
    // 指令和指令行对象均需要更新
}

void Form::tableClick(const QModelIndex &index)
{
    int nr = index.row();
    select_line = nr;
    mLine = nr;
    //QVariant type = index.data(Qt::UserRole);
    //qDebug() << tr("%1 row clicked. data %2").arg(nr+1).arg(type.toInt());

    //itemList->linecmd(mLine);
}

//ABS, RELA, SPD, OPER, JMP, CMP, IOJMP, DELAY, SETOUT, INPUT
void Form::showToolBox(const QModelIndex &index)
{
    int toolbox = index.data(Qt::UserRole).toInt();

    cmdType = toolbox;
    switch (toolbox) {
    case Command::ABS:
        ui->parentToolBox->setCurrentIndex(0);
        ui->moveToolBox->setCurrentIndex(0);
        break;
    case Command::RELA:
        ui->parentToolBox->setCurrentIndex(0);
        ui->moveToolBox->setCurrentIndex(1);
        break;
    case Command::SPD:
        ui->parentToolBox->setCurrentIndex(0);
        ui->moveToolBox->setCurrentIndex(2);
        break;
    case Command::OPER:
        ui->parentToolBox->setCurrentIndex(1);
        ui->operateToolBox->setCurrentIndex(0);
        break;
    case Command::JMP:
        ui->parentToolBox->setCurrentIndex(2);
        ui->condToolBox->setCurrentIndex(0);
        break;
    case Command::CMP:
        ui->parentToolBox->setCurrentIndex(2);
        ui->condToolBox->setCurrentIndex(1);
        break;
    case Command::IOJMP:
        ui->parentToolBox->setCurrentIndex(2);
        ui->condToolBox->setCurrentIndex(1);
        break;
    case Command::INPUT:
        ui->parentToolBox->setCurrentIndex(3);
        ui->ioToolBox->setCurrentIndex(0);
        break;
    case Command::SETOUT:
        ui->parentToolBox->setCurrentIndex(3);
        ui->ioToolBox->setCurrentIndex(1);
        break;
    case Command::DELAY:
        ui->parentToolBox->setCurrentIndex(4);
        ui->auxToolBox->setCurrentIndex(0);
        break;
    default:
        break;
    }
}


void Form::on_clearBtn_clicked()
{
    itemList->clear();
}

void Form::on_deleteBtn_clicked()
{
    if(mLine < 0) {
        QMessageBox::information(this, tr("提示"), tr("请选择指令行！"));
        return;
    }

    itemList->del(mLine);
    qDebug() << tr("%1 行已被删除！").arg(mLine+1);

    mLine = -1;
}

