#include <QApplication>
#include <QtCore/QDebug>
#include <QTableView>
#include "mymodel.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    QTableView view;
    MyModel model(0);

    model.insertRow(2, QModelIndex());
    view.setModel(&model);
    view.show();

    return app.exec();
}
