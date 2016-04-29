#ifndef FORM_H
#define FORM_H

#include <QWidget>


namespace Ui {
class Form;
}

class QStandardItemModel;
class QAbstractItemModel;

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
    //void on_absoluteMove_clicked();

    //void on_relativeMove_clicked();

    //void on_stopMove_clicked();
    void on_absAddBtn_clicked();

    void on_relaAddBtn_clicked();

    void on_resetBtn_clicked();

    void on_autoAct_clicked();

    void on_deleteBtn_clicked();

    void on_stepAct_clicked();

protected:
    void dragEnterEvent(QDragEnterEvent *event); //拖动事件
    void dropEvent(QDropEvent *event); //放置事件

signals:
    void sendData(const QByteArray &data);

private slots:
    void step();

    void on_stopAct_clicked();

private:
    void initUI();
    void initConnect();
    void initModel();

    quint8* convert(int data);
    QByteArray array(quint8*, int size);

private:
    Ui::Form *ui;
    QByteArray echo; //串口返回数据
    //QStandardItemModel *header;
    //QStandardItemModel *model;
    QAbstractItemModel *model;
    QStringList *m_list;
    int row;   //行位置
    int index; //指令序列位置
    QTimer *timer;
    bool status;
};

#endif // FORM_H
