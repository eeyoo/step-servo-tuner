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

class Line : public QObject
{
    Q_OBJECT
public:
    explicit Line(QObject *parent = 0);
    Line(CmdType type, QList<int> params, QObject *parent = 0);
    Line(QStringList &list, QObject *parent = 0);

    void print();
    void strlist(QStringList &list) const;

    void print(QString &str);
    CmdType type();

private:
    QString translate(CmdType type, QString &s);
    void str2key(QString &s);

private:
    CmdType mType;
    QList<int> mParams;
};

#endif // LINE_H
