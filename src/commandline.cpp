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

    line = new Line();
    lines = new QList<Line*>;
}

CommandLine::~CommandLine()
{
    delete model;
    delete line;
    delete lines;
}

void CommandLine::setRowData(int arow, Line *line)
{
    QStringList alist;
    line->strlist(alist);

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
    //line = new Line();
    QTextStream in(&file);
    while(!in.atEnd()) {
        QString str = in.readLine();
        QStringList fields = str.split(" ");

        line = new Line(fields);
        //line->print();
        //Line ln(fields);
        //ln.print();
        //append(&ln);
        append(line);
    }

    //qDebug() << "read file success =============";
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
        lines->at(i)->print(str);

        if (i == rows-1)
            out << str;
        else
            out << str << "\r\n";
    }

    file.close();
    return true;
}

void CommandLine::append(Line *ln)
{
    //qDebug() << QString("row %1 rows %2").arg(row).arg(rows);
    setRowData(row, ln);
    lines->append(ln);
    row++;
    rows++;

}

void CommandLine::insert(Line *ln, int arow)
{
    //row = arow;
    //qDebug() << QString("插入指令行 %1").arg(arow);
    setRowData(arow, ln);
    lines->insert(arow, ln);
    row++;
    rows++;
}

void CommandLine::del(int arow)
{
    //lines.removeAt(arow);
    lines->removeAt(arow);
    model->removeRow(arow, QModelIndex());
    rows--;
    if(rows < 0)
        rows = 0;
    row = rows;
}

QAbstractItemModel *CommandLine::pmodel()
{
    return model;
}

Line* CommandLine::getRowData(int arow) const
{

    //lines.value(arow)
    //line->print();
    //aline = lines->at(arow);
    //line->print();
    //qDebug() << "type -- " << line->type();
    //return lines.value(arow);
    //ln = lines->at(arow);
    //ln.print();
    //lines->at(arow)->print();
    //line->print();
    return lines->at(arow);
}

void CommandLine::clear()
{
    model->removeRows(0, rows, QModelIndex());

    lines->clear();
    row = 0;
    rows = 0;
}

int CommandLine::size() const
{
    return rows;
}

int CommandLine::getRow() const
{
    return row;
}

void CommandLine::show()
{
    //qDebug() << "size -- " << lines->size();
    qDebug() << QString("row %1 rows %2").arg(row).arg(rows);
}

QByteArray CommandLine::getCmdData()
{
    QByteArray qa;

    for (int i=0; i<rows; i++) {
        line = lines->at(i);
        qa.append(line->data());
    }
    //qDebug() << qa.toHex();
    return qa;
}
