#include "shiftmodel.h"

#include <QPointer>

ShiftModel::ShiftModel(QObject* parent) : QAbstractListModel(parent)
{
    //
}

int ShiftModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)

    return m_shiftItems.size();
}

QVariant ShiftModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() >= rowCount())
    {
        return QVariant();
    }

    QPointer<ShiftItem> shiftItem = m_shiftItems.at(index.row());
    if (role == Roles::ObjectRole)
    {
        return QVariant::fromValue(shiftItem.data());
    }

    return QVariant();
}

QHash<int, QByteArray> ShiftModel::roleNames() const
{
    return {{ShiftModel::Roles::ObjectRole, "object"}};
}
