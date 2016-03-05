#ifndef COMMAND_H
#define COMMAND_H

#include <QObject>

/**
 * @brief 指令基类
 */

class Command : public QObject
{
    Q_OBJECT

signals:
    void getData(const QByteArray &data);

public:
    explicit Command();

    void putData(const QByteArray &data);
};

#endif // COMMAND_H
