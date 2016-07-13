#ifndef COMMANDLINE_H
#define COMMANDLINE_H
#include <QAbstractItemModel>

#include "line.h"

class CommandLine : public QObject
{
public:
    CommandLine(QObject *parent = 0);
    ~CommandLine();

    void append(Line *ln);
    void del(int arow);
    void clear();

    bool read(const QString &fileName); //读取程序文件
    bool write(QString &fileName) const; //写入程序文件

    QAbstractItemModel *pmodel();

    Line* getRowData(int arow) const;

    int size() const;

    void show();

private:
    void setRowData(int arow, Line *line);

private:
    QAbstractItemModel *model;
    QList<Line *> *lines;
    Line *line;

    int row;
    int rows;
};

#endif // COMMANDLINE_H
