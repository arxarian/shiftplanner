#include "availabilitymodel.h"

#include <QDebug>
#include <QPointer>

AvailabilityModel::AvailabilityModel(QObject* parent) : QAbstractListModel(parent)
{
    //
}

AvailabilityModel::AvailabilityModel(const QList<QDate> dates, const QStringList& availabilities, QObject* parent)
{
    for (qint32 i = 0; i < dates.count(); ++i)
    {
        append(new AvailabilityItem(dates.at(i), availabilities.at(i), this));
    }
}

int AvailabilityModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)

    return m_items.size();
}

QVariant AvailabilityModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() >= rowCount())
    {
        return QVariant();
    }

    QPointer<AvailabilityItem> pItem = m_items.at(index.row());
    if (role == Roles::ObjectRole)
    {
        return QVariant::fromValue(pItem.data());
    }

    return QVariant();
}

QHash<int, QByteArray> AvailabilityModel::roleNames() const
{
    return {{AvailabilityModel::Roles::ObjectRole, "object"}};
}

void AvailabilityModel::append(AvailabilityItem* item)
{
    if (item)
    {
        beginInsertRows(QModelIndex(), rowCount(), rowCount());
        m_items.append(item);
        endInsertRows();
    }
}

const QString AvailabilityModel::dump() const
{
    QString r;
    for (const auto& item : m_items)
    {
        r.append(QString("%1: %2, ").arg(item->day().toString()).arg(item->availability()));
    }
    return r;
}
