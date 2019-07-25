#ifndef MYMODEL_H
#define MYMODEL_H

#include <QSortFilterProxyModel>


class MyModel : public QSortFilterProxyModel
{
public:
    MyModel();

    // QAbstractItemModel interface
public:
    QVariant data(const QModelIndex &index, int role) const;
};

#endif // MYMODEL_H
