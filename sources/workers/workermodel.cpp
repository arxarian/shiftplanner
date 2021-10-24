#include "workermodel.h"

#include <QPointer>

WorkerModel::WorkerModel(QObject* parent) : QAbstractListModel(parent)
{
    //
}

int WorkerModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)

    return m_items.size();
}

QVariant WorkerModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() >= rowCount())
    {
        return QVariant();
    }

    QPointer<WorkerItem> item = m_items.at(index.row());
    if (role == Qt::DisplayRole)
    {
        return item->name();
    }
    else if (role == Roles::ObjectRole)
    {
        return QVariant::fromValue(item.data());
    }

    return QVariant();
}

QHash<int, QByteArray> WorkerModel::roleNames() const
{
    return {{WorkerModel::Roles::ObjectRole, "object"}};
}

void WorkerModel::append(WorkerItem* item)
{
    if (item)
    {
        beginInsertRows(QModelIndex(), rowCount(), rowCount());
        m_items.append(item);
        endInsertRows();

        item->setParent(this);
    }
}
