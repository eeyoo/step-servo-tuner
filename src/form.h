#ifndef FORM_H
#define FORM_H

#include <QWidget>


namespace Ui {
class Form;
}
class QAbstractItemModel;
class ConfigDialog;
class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(QWidget *parent = 0);
    ~Form();
    void receiveData(const QByteArray &data);

public slots:
    void about();

private slots: 
    //绝对运动
    void on_absAddBtn_clicked();

    void on_relaAddBtn_clicked();

    void on_resetBtn_clicked();

    void on_deleteBtn_clicked();

protected:
    void dragEnterEvent(QDragEnterEvent *event); //拖动事件
    void dropEvent(QDropEvent *event); //放置事件

signals:
    void sendData(const QByteArray &data);

private slots:

    void on_stopAct_clicked();

    void on_forwardAct_clicked(); //打包下载

    void on_stepAct_clicked(); //单步运行

    void on_setSpdBtn_clicked();

    void on_opAddBtn_clicked();

    void on_jmpAddBtn_clicked();

    void on_cmpAddBtn_clicked();

    void on_jumpAddBtn_clicked();

    void on_inputAddBtn_clicked();

    void on_outputAddBtn_clicked();

    void on_delayAddBtn_clicked();

private:
    void initUI();
    void initConnect();
    void initModel();

    void convert(quint8 *buf, int data, int size); //int -> quint8[4]
    void array2qa(QByteArray &data, quint8 *buf, int size); //quint8[4] -> QByteArray

private:
    Ui::Form *ui;
    QByteArray echo; //串口返回数据
    QAbstractItemModel *model;
    int row;        //行位置
    int index;      //指令序列位置
    int jmp_from;   //无条件跳转起始行
    int jmp_to;    //无条件跳转目标行
    long position; //绝对位置
    bool status;
    QList<QByteArray> *cmd_list; //指令列表
    ConfigDialog *config;
    int param; //位移转换脉冲数参数
    quint8 id[2];
};

#endif // FORM_H
