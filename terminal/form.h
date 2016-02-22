#ifndef FORM_H
#define FORM_H

#include <QWidget>

namespace Ui {
class Form;
}

//class MainWindow;

class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(QWidget *parent = 0);
    ~Form();

private slots:
    void about();

signals:
    void closeMe();

private:
    Ui::Form *ui;
    //MainWindow *main;

};

#endif // FORM_H
