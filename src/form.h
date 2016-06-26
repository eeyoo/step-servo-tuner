#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include "commandline.h"
#include "commanditemlist.h"

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

    bool loadProgFile(QString fileName);  //读取json文件并解析出来生成模型和指令序列
    bool saveProgFile(QString fileName) const; //将模型和指令序列保存为json文件

    enum OperType {
        APP, EDIT, INSE, QUIT
    };

public slots:
    void about();

protected:
    void dragEnterEvent(QDragEnterEvent *event); //拖动事件
    void dropEvent(QDropEvent *event); //放置事件

signals:
    void sendData(const QByteArray &data);

private slots:

    void on_absAddBtn_clicked(); //绝对运动

    void on_relaAddBtn_clicked(); //相对运动

    void on_stopAct_clicked();    //停止

    void on_forwardAct_clicked(); //打包下载

    //void on_stepAct_clicked(); //单步运行

    void on_setSpdBtn_clicked();

    void on_opAddBtn_clicked();

    void on_jmpAddBtn_clicked();

    void on_cmpAddBtn_clicked();

    void on_jumpAddBtn_clicked();

    void on_inputAddBtn_clicked();

    void on_outputAddBtn_clicked();

    void on_delayAddBtn_clicked();

    void spd_show(int);

    void tableDoubleClick(const QModelIndex &); //双击事件

    void tableClick(const QModelIndex &index);  //单击事件

    void showToolBox(const QModelIndex &index);



private slots:

    void on_clearBtn_clicked();  //清空

    void on_deleteBtn_clicked(); //删除

    void on_insertBtn_clicked(); //插入

private:
    void initUI();
    void initConnect();
    void initModel();

    void operate(Command &cmd, QStringList &list);

    //void convert(quint8 *buf, int data, int size); //int -> quint8[4]
    //void array2qa(QByteArray &data, quint8 *buf, int size); //quint8[4] -> QByteArray

private:
    Ui::Form *ui;
    CommandItemList *itemList;
    Command *cmd;
    int index;      //单步运行行
    ConfigDialog *config;
    int select_line;  //选中当前行
    int deviceId;
    int cmdType;
    OperType op;
    bool quit;
    int mLine;
};

#endif // FORM_H
