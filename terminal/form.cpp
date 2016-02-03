#include "form.h"
#include "ui_form.h"

#include "mainwindow.h"

Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);
    //main = new MainWindow();

    //connect(ui->pbStart, SIGNAL(clicked()), this, SLOT());
    connect(ui->pbStart, SIGNAL(clicked(bool)), this, SIGNAL(closeMe()));
}

Form::~Form()
{
    delete ui;
}

void Form::about() {
    //Ui::MainWindow *main = new MainWindow();

}
