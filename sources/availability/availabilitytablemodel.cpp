#include "availabilitytablemodel.h"
#include "availabilityitem.h"
#include "availabilitymodel.h"

#include <QDebug>
#include <QPointer>

AvailabilityTableModel::AvailabilityTableModel(QObject* parent) : QAbstractTableModel(parent)
{
    //
}

int AvailabilityTableModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)

    return m_workers.count();
}

int AvailabilityTableModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)

    //    if (m_workers->rowCount() > 0)
    //    {
    //        WorkerItem* worker = qvariant_cast<WorkerItem*>(m_workers->data(m_workers->index(0), WorkerModel::ObjectRole));
    //        if (worker->availability())
    //        {
    //            return worker->availability()->rowCount();
    //        }
    //    }

    return m_dates.count();
}

QVariant AvailabilityTableModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() >= rowCount(QModelIndex()) || index.column() >= columnCount(QModelIndex()))
    {
        return QVariant();
    }

    //    AvailabilityModel* model       = qvariant_cast<WorkerItem*>(m_workers->data(index, WorkerModel::ObjectRole))->availability();
    //    AvailabilityItem* availability = qvariant_cast<AvailabilityItem*>(model->data(model->index(index.column()), AvailabilityModel::ObjectRole));

    if (role == Qt::DisplayRole)
    {
        return m_workersAvailabitilty.value(m_workers.at(index.row())).at(index.column());
    }

    return QVariant();
}

QVariant AvailabilityTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal)
        {
            if (section < m_dates.count())
            {
                return m_dates.at(section);
            }
            //            WorkerItem* worker              = qvariant_cast<WorkerItem*>(m_workers->data(m_workers->index(0), WorkerModel::ObjectRole));
            //            AvailabilityModel* availability = worker->availability();
            //            AvailabilityItem* item =
            //              qvariant_cast<AvailabilityItem*>(availability->data(availability->index(section), AvailabilityModel::ObjectRole));
            //            return item->day().toString("d. M.");
        }
        else
        {
            if (section < m_workers.count())
            {
                return m_workers.at(section);
            }
            //            return m_workers->data(m_workers->index(section));
        }
    }
    return QVariant();
}

QStringList AvailabilityTableModel::workers() const
{
    return m_workers;
}

QStringList AvailabilityTableModel::dates() const
{
    return m_dates;
}

QMap<QString, QStringList> AvailabilityTableModel::workersAvailabitilty()
{
    return m_workersAvailabitilty;
}

void AvailabilityTableModel::setAvailability(const QString& availabilityRaw)
{
    QStringList&& rows = availabilityRaw.split(QChar::LineFeed);
    qDebug() << "rows" << rows;
    //    QString&& horizontalHeader = rows.takeFirst();
    //    //    rows.removeFirst(); // useless data

    //    QStringList&& dates = horizontalHeader.split(QChar::Tabulation);
    //    dates.removeFirst();
    //    m_availabilityModel->setDates(dates);
    //    m_availabilityModel->setWorkersAvailabitilty(rows);
}

// WorkerModel* AvailabilityTableModel::workers() const
//{
//    return m_workers;
//}

// void AvailabilityTableModel::setWorkers(WorkerModel* workers)
//{
//    if (m_workers == workers)
//        return;

//    m_workers = workers;

//    connect(m_workers, &QAbstractItemModel::columnsInserted, this, &AvailabilityTableModel::columnsInserted);
//    connect(m_workers, &QAbstractItemModel::columnsRemoved, this, &AvailabilityTableModel::columnsRemoved);
//    connect(m_workers, &QAbstractItemModel::rowsInserted, this, &AvailabilityTableModel::rowsInserted);
//    connect(m_workers, &QAbstractItemModel::rowsRemoved, this, &AvailabilityTableModel::rowsRemoved);
//    connect(m_workers, &QAbstractItemModel::modelReset, this, &AvailabilityTableModel::modelReset);

//    emit workersChanged(m_workers);
//}

void AvailabilityTableModel::setDates(const QStringList& dates)
{
    m_dates = dates;
    m_dates.removeAll("");

    emit headerDataChanged(Qt::Horizontal, 0, m_dates.count() - 1);
    //    // extract dates
    //    QList<QDate> dates;
    //    for (const QString& rawDate : header)
    //    {
    //        if (!rawDate.isEmpty())
    //        {
    //            const QStringList& dayAndMonth = rawDate.split(QChar('.'));
    //            dates.append(QDate(QDate::currentDate().year(), dayAndMonth.at(1).toInt(), dayAndMonth.at(0).toInt()));
    //        }
    //    }
}

void AvailabilityTableModel::setWorkersAvailabitilty(const QStringList& workersAvailabitilty)
{
    m_workers.clear();
    m_workersAvailabitilty.clear();

    for (const QString& row : workersAvailabitilty)
    {
        if (row.isEmpty())
        {
            continue;
        }

        QStringList workerAvailability = row.split(QChar::Tabulation);
        m_workers.append(workerAvailability.takeFirst());

        m_workersAvailabitilty[m_workers.last()] = workerAvailability;
    }

    emit headerDataChanged(Qt::Vertical, 0, m_workers.count() - 1);
    emit dataChanged(index(0, 0), index(m_workers.count() - 1, m_dates.count() - 1));
    emit workersChanged(m_workers);

    //// create worker list
    // for (const QString& row : rows)
    //{
    //    AvailabilityModel* availabilities = new AvailabilityModel(dates, row.mid(row.indexOf(QChar::Tabulation) + 1).split(QChar::Tabulation));
    //    const QString& worker             = row.left(row.indexOf(QChar::Tabulation));
    //    m_workers->append(new WorkerItem(worker, 80, availabilities));
    //}
    //}
}
