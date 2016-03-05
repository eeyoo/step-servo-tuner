#ifndef FORM_H
#define FORM_H

#include <QWidget>

namespace Ui {
class Form;
}

class Command;
class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(QWidget *parent = 0);
    ~Form();

private slots:
    void about();
    void emergencyStop();

signals:
    void closeMe();
    void getData(const QByteArray &data);

private:
    void initUI();
    void initConnect();

private:
    Ui::Form *ui;
    Command *comm;
};

#endif // FORM_H
