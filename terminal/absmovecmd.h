#ifndef ABSMOVECMD_H
#define ABSMOVECMD_H
#include "command.h"

class AbsMoveCmd : public Command
{
public:
    AbsMoveCmd(Cmd *cmd);
    QByteArray getData() const;
};

#endif // ABSMOVECMD_H
