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
    //connect(ui->pbStart, SIGNAL(clicked(bool)), this, SIGNAL(closeMe()));

    ui->elecGear->addItem(QWidget::tr("高"));
    ui->elecGear->addItem(QWidget::tr("低"));

    ui->backHomeDirection->addItem(QWidget::tr("正向"));
    ui->backHomeDirection->addItem(QWidget::tr("反向"));

    ui->backHomeGear->addItem(QWidget::tr("快档"));
    ui->backHomeGear->addItem(QWidget::tr("慢档"));
}

Form::~Form()
{
    delete ui;
}

void Form::about() {
    //Ui::MainWindow *main = new MainWindow();

}
