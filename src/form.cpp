#include "form.h"
#include "ui_form.h"

#include "command.h"
#include "configdialog.h"
#include "line.h"

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
    row(-1),
    ui(new Ui::Form)
{
    ui->setupUi(this);
    config = new ConfigDialog;
    //itemList = new CommandItemList(this);

    int level = config->configs().elecLevel;
    int circle = config->configs().circleLen;
    int div[] = {1,2,4,8,16,32,64,128,256};
    deviceId = config->configs().deviceId;

    int alpha = 200 * div[level] / circle;
    double beta = 0.4 * div[level] / circle; // 系数 200*100/50000

    int maxSpd = 100 / beta;
    ui->setRunSpd->setMaximum(maxSpd);

    //cmd = new Command(alpha, beta);

    initUI();
    initConnect();
    initModel();

    spd_show(ui->setRunSpd->value());

    ui->parentToolBox->setCurrentIndex(0);
    ui->moveToolBox->setCurrentIndex(0);

    cmdType = -1;
    op = APP;
    quit = false;

    line = new Line(alpha, beta, deviceId);
    cl = new CommandLine(this);
}

Form::~Form()
{
    delete ui;
    delete config;
    delete cl;
    delete line;
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

bool Form::saveProgFile(QString fileName) const
{
    return cl->write(fileName);
}

bool Form::loadProgFile(QString fileName)
{

    bool ret = cl->read(fileName);
    ui->tableView->setModel(cl->pmodel());
    return ret;
}
/*
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
*/
void Form::operate(Line *ln)
{
    switch (op) {
    case APP: //默认追加
        //qDebug() << "========= APP ===========";
        //指令序列 - 模型
        cl->append(ln);
        break;
    case EDIT://指令修改
        //qDebug() << "========= EDIT ===========";
        break;
    case INSE://指令插入 默认前查
        //qDebug() << "========= INSE ===========";
        //itemList->insert(cmd, list, mLine);
        //mLine++;
        break;
    default:
        break;
    }

    ui->tableView->setModel(cl->pmodel());
}

void Form::operate(QList<int> pa, CmdType type)
{
    line = new Line(pa, type);

    switch (op) {
    case APP: //默认追加
        cl->append(line);
        break;
    case EDIT://指令修改
        break;
    case INSE://指令插入 默认前查
        break;
    default:
        break;
    }

    ui->tableView->setModel(cl->pmodel());
}

void Form::on_absAddBtn_clicked()
{

    int position = ui->absMoveDistance->value();

    //int params[2] = {deviceId, position};
    //Command acmd(params, Command::ABS);
    //acmd.pp();
    //qDebug() << acmd.data().toHex();

    QList<int> pa;
    pa << position;

    operate(pa, POS);
}

void Form::on_relaAddBtn_clicked()
{
    int pos = ui->relMoveDistance->value();
    //int base = itemList->pos();//获取指令序列当前绝对位置

    //int params[3] = {deviceId, pos, base}; //绝对位置 = 增量 + 基准
    //Command acmd(params, Command::RELA);
    //qDebug() << acmd.data().toHex();

    QList<int> pa;
    pa << pos;
    operate(pa, MOV);

    //QStringList list;
    //list << tr("相对运动指令") << QString(tr("相对运行距离 %1mm")).arg(pos);

    //operate(acmd, list);
}

void Form::on_setSpdBtn_clicked()
{
    //moves.append(0);

    int lpd = ui->setRunSpd->value(); //线速度mm/s - 转速 - 每秒脉冲数

    QList<int> pa;
    pa << lpd;

    operate(pa, SETSPD);

}

void Form::on_delayAddBtn_clicked()
{

    int value = ui->delayVal->value();

    QList<int> pa;
    pa << value;
    operate(pa, DELAY);
}

void Form::on_stepAct_clicked()
{
    //cl->show();

}

void Form::spd_show(int lpd)
{
    int circle = config->configs().circleLen;
    //int lpd = ui->setRunSpd->value();
    double rps = (double)lpd / circle;
    ui->rpsLab->setText(QString(tr("转速：%1 rps(转每秒)")).arg(rps));
}

void Form::on_stopAct_clicked()
{
    //int params[2] = {deviceId, 0};
    //Command stopcmd(params, Command::STOP);

    QList<int> pa;
    pa << 0;
    //operate(pa, DELAY);
    line = new Line(pa, STOP);

    qDebug() << line->data().toHex();
    //emit sendData(stopcmd.data());
}

void Form::on_forwardAct_clicked()
{
    /*
    int len = itemList->size();

    int params[2] = {deviceId, len};
    Command acmd(params, Command::HEAD);
    qDebug() << acmd.data().toHex();

    QByteArray qa = acmd.data();
    itemList->output(qa);
    qDebug() << qa.toHex();
    */
    //qDebug() << acmd.data().append(qa).toHex();
    //emit sendData(qa);
    //cl->getCmdData();
    QByteArray qa;
    QList<int> pa;
    pa << cl->size();
    line = new Line(pa, HEAD);
    qa.append(line->data());
    qa.append(cl->getCmdData());
    qDebug() << qa.toHex();
}

void Form::on_opAddBtn_clicked()
{
    //moves.append(0);
    int param = ui->opParam->value();
    int opType = ui->opType->currentIndex(); //1-自增 2-自减

    QList<int> pa;
    pa << param << opType;

    operate(pa, OPER);
}

void Form::on_jmpAddBtn_clicked()
{
    int val = ui->jmpLine->value();

    int rows = cl->size();
    //qDebug() << tr("无条件跳转至 %1 行，总行数 %2").arg(line).arg(rows);

    if(val > rows) {
        QMessageBox::warning(this, tr("警告"), QString(tr("跳转行不能超过 %1 行")).arg(rows));
        return;
    }

    QList<int> pa;
    pa << val;

    operate(pa, JMP);
}

void Form::on_cmpAddBtn_clicked()
{
    //moves.append(0);
    int val = ui->cmpLine->value();
    int rows = cl->size();

    if(val > rows) {
        QMessageBox::warning(this, tr("警告"), QString(tr("跳转行不能超过 %1 行")).arg(rows));
        return;
    }

    int param = ui->cmpParam->value();
    int type = ui->cmpType->currentIndex();
    int value = ui->cmpVal->value();

    QList<int> pa;
    pa << param << type << value << val;

    operate(pa, CMP);
}

void Form::on_jumpAddBtn_clicked()
{
    int val = ui->jumpLine->value();
    int rows = cl->size();
    if(val > rows) {
        QMessageBox::warning(this, tr("警告"), QString(tr("跳转行不能超过 %1 行")).arg(rows));
        return;
    }

    int param = ui->jumpParam->value();
    int state = ui->ioState->currentIndex();//0-低电平 1-高电平

    QList<int> pa;
    pa << param << state << val;

    operate(pa,IOJMP);
}

void Form::on_inputAddBtn_clicked() //输入等待
{
    int param = ui->inputParam->value();
    int state = ui->inputState->currentIndex();

    QList<int> pa;
    pa << param << state;

    operate(pa, INPUT);
}

void Form::on_outputAddBtn_clicked() //主动输出
{
    int param = ui->outputParam->value();
    int state = ui->outputState->currentIndex();

    QList<int> pa;
    pa << param << state;
    operate(pa, SETOUT);
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
    row = nr;

    line = cl->getRowData(row);
    line->print();
}

//ABS, RELA, SPD, OPER, JMP, CMP, IOJMP, DELAY, SETOUT, INPUT
void Form::showToolBox(const QModelIndex &index)
{
    //int toolbox = index.data(Qt::UserRole).toInt();

    line = cl->getRowData(index.row());


    switch (line->type()) {
    case POS:
        ui->parentToolBox->setCurrentIndex(0);
        ui->moveToolBox->setCurrentIndex(0);
        break;
    case MOV:
        ui->parentToolBox->setCurrentIndex(0);
        ui->moveToolBox->setCurrentIndex(1);
        break;
    case SETSPD:
        ui->parentToolBox->setCurrentIndex(0);
        ui->moveToolBox->setCurrentIndex(2);
        break;
    case OPER:
        ui->parentToolBox->setCurrentIndex(1);
        ui->operateToolBox->setCurrentIndex(0);
        break;
    case JMP:
        ui->parentToolBox->setCurrentIndex(2);
        ui->condToolBox->setCurrentIndex(0);
        break;
    case CMP:
        ui->parentToolBox->setCurrentIndex(2);
        ui->condToolBox->setCurrentIndex(1);
        break;
    case IOJMP:
        ui->parentToolBox->setCurrentIndex(2);
        ui->condToolBox->setCurrentIndex(2);
        break;
    case INPUT:
        ui->parentToolBox->setCurrentIndex(3);
        ui->ioToolBox->setCurrentIndex(0);
        break;
    case SETOUT:
        ui->parentToolBox->setCurrentIndex(3);
        ui->ioToolBox->setCurrentIndex(1);
        break;
    case DELAY:
        ui->parentToolBox->setCurrentIndex(4);
        ui->auxToolBox->setCurrentIndex(0);
        break;
    default:
        break;
    }
}


void Form::on_clearBtn_clicked()
{
    cl->clear();
}

void Form::on_deleteBtn_clicked()
{
    if(row < 0) {
        QMessageBox::information(this, tr("提示"), tr("请选择指令行！"));
        return;
    }

    cl->del(row);
    qDebug() << tr("%1 行已被删除！").arg(row+1);

    row = -1;
}

void Form::on_insertBtn_clicked()
{
    //默认选中行之前插入指令
    if(row < 0) {
        QMessageBox::information(this, tr("提示"), tr("请选择指令行！"));
        return;
    }

    if (quit) {
        op = APP;
        row = -1;
        ui->insertBtn->setText(tr("插入"));
        quit = false;
    }

    if (row >= 0) {
        QMessageBox::information(this, tr("提示"), tr("将在选中行之前插入指令，可以连续插入！"));
        ui->insertBtn->setText(tr("停止插入"));
        op = INSE;
        quit = true; //允许取消
    }
}

