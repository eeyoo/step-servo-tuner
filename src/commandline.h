#ifndef COMMANDLINE_H
#define COMMANDLINE_H
#include <QString>
#include <QByteArray>
#include <QJsonObject>

class CommandLine
{
public:
    CommandLine();
    CommandLine(const QString &type, const QString &content, const QByteArray &data);

    void read(const QJsonObject &json);
    void write(QJsonObject &json) const;

    void print();
    QString type() const;
    QString content() const;
    QByteArray data() const;

private:
    QString mType;
    QString mContent;
    QByteArray mData;
};

#endif // COMMANDLINE_H
