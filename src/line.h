#ifndef LINE_H
#define LINE_H

/**
 * @brief The line class
 * 指令行
 */
#include <QObject>

enum CmdType {
    POS, MOV, SETSPD, OPER, JMP, CMP, IOJMP, DELAY, SETOUT, INPUT
};

class Line
{
public:
    explicit Line();
    Line(CmdType type, int *params);
    Line(QStringList &list);

    void print();
    void strlist(QStringList &list) const;

    void print(QString &str);

private:
    QString translate(CmdType type, QString &s);

private:
    CmdType mType;
    int *mParams;
};

#endif // LINE_H
