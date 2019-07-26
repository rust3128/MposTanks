#include "mymodel.h"
#include <QColor>
#include <QString>
#include <QDateTime>

MyModel::MyModel()
{

}


QVariant MyModel::data(const QModelIndex &index, int role) const
{
    int column = index.column();
    if(role == Qt::BackgroundRole){
        if(column >=4 && column <= 7 )  return QColor("#A9BCF5");
        if(column >=8 && column <= 11 )  return QColor("#F78181");
        if(column == 12) return QColor("#FFFF00");
        if(column == 13) return QColor("#3ADF00");
    }

    if(role == Qt::DisplayRole){
        if(column == 1) return QSortFilterProxyModel::data(index,Qt::DisplayRole).toDateTime().toString("dd.MM.yyyy hh:mm:ss");
        if(column >=4 ){
            return QString::number(QSortFilterProxyModel::data(index,Qt::DisplayRole).toDouble(),'f',2);
        }

    }
    if(role == Qt::TextAlignmentRole){
        if(column == 0 || column == 3 ) return Qt::AlignHCenter;
        if(column >=4 ) return Qt::AlignRight;
    }

    return QSortFilterProxyModel::data(index, role);
}
