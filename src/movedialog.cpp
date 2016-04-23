#include "movedialog.h"
#include "ui_movedialog.h"

MoveDialog::MoveDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MoveDialog)
{
    ui->setupUi(this);
}

MoveDialog::~MoveDialog()
{
    delete ui;
}

/*
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
*/
