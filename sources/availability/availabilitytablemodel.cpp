#include "availabilitytablemodel.h"
#include "availabilityitem.h"
#include "availabilitymodel.h"

#include <QPointer>

AvailabilityTableModel::AvailabilityTableModel(QObject* parent) : QAbstractTableModel(parent)
{
    //
}

int AvailabilityTableModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)

    return m_workers->rowCount();
}

int AvailabilityTableModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)

    if (m_workers->rowCount() > 0)
    {
        WorkerItem* worker = qvariant_cast<WorkerItem*>(m_workers->data(m_workers->index(0), WorkerModel::ObjectRole));
        if (worker->availability())
        {
            return worker->availability()->rowCount();
        }
    }

    return 0;
}

QVariant AvailabilityTableModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() >= rowCount(QModelIndex()) || index.column() >= columnCount(QModelIndex()))
    {
        return QVariant();
    }

    AvailabilityModel* model       = qvariant_cast<WorkerItem*>(m_workers->data(index, WorkerModel::ObjectRole))->availability();
    AvailabilityItem* availability = qvariant_cast<AvailabilityItem*>(model->data(model->index(index.column()), AvailabilityModel::ObjectRole));

    if (role == Qt::DisplayRole)
    {
        return availability->toString();
    }

    return QVariant();
}

QVariant AvailabilityTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal)
        {
            WorkerItem* worker              = qvariant_cast<WorkerItem*>(m_workers->data(m_workers->index(0), WorkerModel::ObjectRole));
            AvailabilityModel* availability = worker->availability();
            AvailabilityItem* item =
              qvariant_cast<AvailabilityItem*>(availability->data(availability->index(section), AvailabilityModel::ObjectRole));
            return item->day().toString("d. M.");
        }
        else
        {
            return m_workers->data(m_workers->index(section));
        }
    }
    return QVariant();
}

WorkerModel* AvailabilityTableModel::workers() const
{
    return m_workers;
}

void AvailabilityTableModel::setWorkers(WorkerModel* workers)
{
    if (m_workers == workers)
        return;

    m_workers = workers;

    connect(m_workers, &QAbstractItemModel::columnsInserted, this, &AvailabilityTableModel::columnsInserted);
    connect(m_workers, &QAbstractItemModel::columnsRemoved, this, &AvailabilityTableModel::columnsRemoved);
    connect(m_workers, &QAbstractItemModel::rowsInserted, this, &AvailabilityTableModel::rowsInserted);
    connect(m_workers, &QAbstractItemModel::rowsRemoved, this, &AvailabilityTableModel::rowsRemoved);
    connect(m_workers, &QAbstractItemModel::modelReset, this, &AvailabilityTableModel::modelReset);

    emit workersChanged(m_workers);
}
