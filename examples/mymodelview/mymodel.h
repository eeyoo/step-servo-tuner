#ifndef MYMODEL_H
#define MYMODEL_H
#include <QAbstractTableModel>
#include <QTimer>

#include <QObject>

const int ROWS = 2;
const int COLS = 3;
class MyModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    MyModel(QObject *parent);
    //MyModel(int, int, QObject *parent=0);
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    Qt::ItemFlags flags(const QModelIndex &index) const;

signals:
    void editCompleted(const QString&);

private slots:
    void timeHit(); //定时器触发

private:
    QTimer *timer;
    QString m_gridData[ROWS][COLS];
    static int m_row;
    //int m_col;
};

#endif // MYMODEL_H
