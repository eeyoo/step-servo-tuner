#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include "commandline.h"

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

    bool loadProgFile(QString fileName);
    bool saveProgFile(QString fileName) const;

    enum OperType {
        APP, EDIT, INSE, QUIT
    };
    //更新配置参数
    void updateConfigs(int level, int circle, int deviceId);

public slots:
    void about();

signals:
    void sendData(const QByteArray &data);
    void sendStop(const QByteArray &data);

private slots:

    void on_absAddBtn_clicked(); //绝对运动

    void on_relaAddBtn_clicked(); //相对运动

    void on_stopAct_clicked();    //停止

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

    void on_homeAddBtn_clicked();

    void spd_show(int);

    void tableDoubleClick(const QModelIndex &); //双击事件

    void tableClick(const QModelIndex &index);  //单击事件

    void showToolBox(const QModelIndex &index);


private slots:
    void on_clearBtn_clicked();  //清空
    void on_deleteBtn_clicked(); //删除
    void on_insertBtn_clicked(); //插入
    void on_editBtn_clicked();   //修改

private:
    void initUI();
    void initConnect();
    void initModel();

    //void operate(Command &cmd, QStringList &list);

    void operate(Line *ln);
    void operate(QList<int> pa, CmdType type);

private:
    Ui::Form *ui;
    CommandLine *cl;
    Line *line;
    ConfigDialog *config;

    OperType op;
    bool quit;
    int row; //选中行
    int runLine; //单步执行行
    int insertLine; //插入指令行
    int updateLine; //修改指令行
};

#endif // FORM_H
