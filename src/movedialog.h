#ifndef MOVEDIALOG_H
#define MOVEDIALOG_H

#include <QDialog>

namespace Ui {
class MoveDialog;
}

class MoveDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MoveDialog(QWidget *parent = 0);
    ~MoveDialog();

private:
    Ui::MoveDialog *ui;
};

#endif // MOVEDIALOG_H
