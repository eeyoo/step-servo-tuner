#include "commandline.h"
#include <QtCore/QDebug>

CommandLine::CommandLine()
{

}

CommandLine::CommandLine(const QString &type, const QString &content, const QByteArray &data)
{
    mType = type;
    mContent = content;
    mData = data;
}

void CommandLine::read(const QJsonObject &json)
{
    mType = json["type"].toString();
    mContent = json["content"].toString();
    mData = json["data"].toString().toLocal8Bit(); //QString -> QByteArray
}

void CommandLine::write(QJsonObject &json) const
{
    json["type"] = mType;
    json["content"] = mContent;
    mData.toHex();
    //qDebug() << "data: " + mData.toHex();
    json["data"] = QString::fromLocal8Bit(mData.toHex().data());
}

void CommandLine::print()
{

}

QString CommandLine::type() const
{
    return mType;
}

QString CommandLine::content() const
{
    return mContent;
}

QByteArray CommandLine::data() const
{
    return mData;
}
