#include "operatormodel.h"

#include <QPointer>

OperatorModel::OperatorModel(QObject* parent) : QAbstractListModel(parent)
{
    //
}

int OperatorModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)

    return m_arrOperatorItems.size();
}

QVariant OperatorModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() >= rowCount())
    {
        return QVariant();
    }

    QPointer<OperatorItem> pOperatorItem = m_arrOperatorItems.at(index.row());
    if (role == Roles::ObjectRole)
    {
        return QVariant::fromValue(pOperatorItem.data());
    }

    return QVariant();
}

QHash<int, QByteArray> OperatorModel::roleNames() const
{
    return {{OperatorModel::Roles::ObjectRole, "object"}};
}
