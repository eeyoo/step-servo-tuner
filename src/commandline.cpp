#include "commandline.h"

#include <QFile>
#include <QStandardItemModel>
#include <QtCore/QDebug>

CommandLine::CommandLine(QObject *parent) :
    QObject(parent), row(0), rows(0)
{
    model = new QStandardItemModel(0, 2, parent);
    model->setHeaderData(0, Qt::Horizontal, tr("指令类型"));
    model->setHeaderData(1, Qt::Horizontal, tr("指令内容"));

    lines = new QList<Line>();
}

CommandLine::~CommandLine()
{
    delete model;
    delete lines;
}

void CommandLine::setRowData(int arow, Line aline)
{
    QStringList alist;
    //line->strlist(alist);
    aline.strlist(alist);

    model->insertRow(arow, QModelIndex());
    model->setData(model->index(arow, 0), alist.at(0), Qt::DisplayRole);
    model->setData(model->index(arow, 1), alist.at(1), Qt::DisplayRole);
}


bool CommandLine::read(const QString &fileName)
{
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "open program file to read failed.";
        return false;
    }

    clear();
    //qDebug() << QString("row %1 rows %2").arg(row).arg(rows);

    QTextStream in(&file);
    while(!in.atEnd()) {
        QString str = in.readLine();
        QStringList fields = str.split(" ");
        Line aline(fields);
        append(aline);
        //qDebug() << "== append line ==";
    }

    file.close();
    return true;
}

bool CommandLine::write(QString &fileName) const
{
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly)) {
        qDebug() << "open program file to write failed.";
        return false;
    }

    QTextStream out(&file);

    for(int i=0; i<rows; i++)
    {
        QString str;
        //lines.at(i)->print(str);
        //qDebug() << str;
        //lines->at(i).print(str);
        Line ln = lines->at(i);
        ln.print(str);
        out << str << "\r\n";
    }

    file.close();
    return true;
}

void CommandLine::append(Line aline)
{
    //line = aline;
    //line->print();
    setRowData(row, aline);
    //lines.append(aline);
    lines->append(aline);
    row++;
    rows++;
    //lines.at(row-1)->print();
}

void CommandLine::del(int arow)
{
    //lines.removeAt(arow);
    lines->removeAt(arow);
    model->removeRow(arow, QModelIndex());
    rows--;
    if (rows < 0)
        rows = 0;
    row = rows;
}

QAbstractItemModel *CommandLine::pmodel()
{
    return model;
}

void CommandLine::getRowData(int arow, Line &aline)
{
    //line = lines.at(arow);
    //line->print();
    aline = lines->at(arow);
    //return lines->at(arow);
}

void CommandLine::clear()
{
    model->removeRows(0, rows, QModelIndex());
    //lines.clear();
    lines->clear();
    row = 0;
    rows = 0;
}

int CommandLine::size() const
{
    return rows;
}
