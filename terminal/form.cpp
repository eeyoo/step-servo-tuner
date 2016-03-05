#include "form.h"
#include "ui_form.h"

#include "command.h"

#include <QtCore/QDebug>

Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);
    comm = new Command;
    /*
    console = new Console();
    console->setEnabled(false);
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(console);
    ui->tab_terminal->setLayout(layout);
    */
    initUI();
    initConnect();
}

Form::~Form()
{
    delete ui;
}

void Form::about()
{

}

void Form::initUI()
{
    ui->elecGear->addItem(QWidget::tr("高"));
    ui->elecGear->addItem(QWidget::tr("低"));

    ui->backHomeDirection->addItem(QWidget::tr("正向"));
    ui->backHomeDirection->addItem(QWidget::tr("反向"));

    ui->backHomeGear->addItem(QWidget::tr("快档"));
    ui->backHomeGear->addItem(QWidget::tr("慢档"));
}

void Form::emergencyStop()
{

    QByteArray data = "abcd";

    emit getData(data);
    qDebug() << "Form send command " + QString(data);
}

void Form::initConnect()
{
    connect(ui->emergencyButton, SIGNAL(clicked()), SLOT(emergencyStop()));
    //connect(ui->emergencyButton, SIGNAL(clicked()), SIGNAL(getData(QByteArray)));
}
