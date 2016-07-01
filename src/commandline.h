#ifndef COMMANDLINE_H
#define COMMANDLINE_H
#include <QAbstractItemModel>

#include "line.h"

class CommandLine : public QObject
{
public:
    CommandLine(QObject *parent = 0);
    ~CommandLine();

    void append(Line aline);
    void del(int arow);
    void clear();

    bool read(const QString &fileName); //读取程序文件
    bool write(QString &fileName) const; //写入程序文件

    QAbstractItemModel *pmodel();

    void getRowData(int arow, Line &aline);

    int size() const;

private:
    void setRowData(int arow, Line aline);

private:
    QAbstractItemModel *model;
    QList<Line> *lines;
    //Line line;

    int row;
    int rows;
};

#endif // COMMANDLINE_H
