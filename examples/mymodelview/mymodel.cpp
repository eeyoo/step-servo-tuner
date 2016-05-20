#include "mymodel.h"

#include <QtCore/QDebug>
#include <QFont>
#include <QBrush>
#include <QTime>

int MyModel::m_row = 0;

MyModel::MyModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    //m_row = 0;
    //m_col = 0;
    timer = new QTimer(this);
    timer->setInterval(1000);
    connect(timer, SIGNAL(timeout()), this, SLOT(timeHit()));

    timer->start();
}

int MyModel::rowCount(const QModelIndex &/*parent*/) const
{
    return 2;
}

int MyModel::columnCount(const QModelIndex &/*parent*/) const
{
    return 3;
}

QVariant MyModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role==Qt::DisplayRole)
    {
        if (orientation==Qt::Horizontal)
        {
            switch (section) {
            case 0:
                return tr("第一列");
            case 1:
                return tr("第二列");
            case 2:
                return tr("第三列");

            default:
                break;
            }
        }
    }
    return QVariant();
}

QVariant MyModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    int col = index.column();

/*
    qDebug() << QString("row%1, col%2, role%3")
                .arg(row).arg(col).arg(role);

    switch (role) {
    case Qt::DisplayRole:
        if(row==0 && col==1) return QString("<--left");
        if(row==1 && col==1) return QString("right-->");
        return QString("Row%1, Column%2").arg(row+1).arg(col+1);
        break;
    case Qt::FontRole:
        if(row==0 && col==0)
        {
            QFont boldFont;
            boldFont.setBold(true);
            return boldFont;
        }
        break;
    case Qt::BackgroundRole:
        if (row==1 && col==2)
        {
            QBrush redBackgroudRed(Qt::red);
            return redBackgroudRed;
        }
        break;
    case Qt::TextAlignmentRole:
        if (row==1 && col==1)
        {
            return Qt::AlignRight + Qt::AlignVCenter;
        }
        break;
    case Qt::CheckStateRole:
        if (row==1 && col==0)
        {
            return Qt::Checked;
        }
    default:
        break;
    }
*/

    if (row==m_row && col==0)
    {
        //return QString("Row%1, Col%2").arg(row).arg(col);
    }

    return QVariant();
}

bool MyModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role==Qt::EditRole)
    {
        m_gridData[index.row()][index.column()] = value.toString();
        QString result;
        for (int row=0; row < ROWS; row++)
        {
            for (int col=0; col < COLS; col++)
            {
                result += m_gridData[row][col] + " ";
            }
        }
        qDebug() << result;
        emit editCompleted( result );
    }
    return true;
}

Qt::ItemFlags MyModel::flags(const QModelIndex &/*index*/) const
{
    return Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled;
}

void MyModel::timeHit()
{

    qDebug() << "row = " << m_row;
    setData(index(m_row, 0, QModelIndex()), QString("Row_%1").arg(m_row), Qt::DisplayRole);
}
