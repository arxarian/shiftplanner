#include "scheduletablemodel.h"

#include "../global.h"

#include <QDebug>
#include <QPointer>

ScheduleTableModel::ScheduleTableModel(QObject* parent) : QAbstractTableModel(parent)
{
    m_workersCount.resize(G::ShiftsCount);
}

int ScheduleTableModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)

    return m_rows;
}

int ScheduleTableModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)

    return m_columns;
}

QVariant ScheduleTableModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() >= rowCount(QModelIndex()) || index.column() >= columnCount(QModelIndex()))
    {
        return QVariant();
    }

    if (role == Qt::DisplayRole)
    {
        if (index.column() == 0)
        {
            return "name";
        }
        else
        {
            const qint32 slot = (index.column() - 1) * 2;
            qDebug() << "*** slot" << slot;
        }
        // slots / shifts / names

        //        if (index.column() == 0)
        //        {
        //            return 0;
        //        }
        //        else if (index.row() == 0)
        //        {
        //            return m_dates.at(index.column() - 1);
        //        }
        //        else
        //        {
        //            return 5;
        //        }
    }

    return QVariant();
}

QVariant ScheduleTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_UNUSED(orientation)

    if (role == Qt::DisplayRole)
    {
        return section;
    }
    return QVariant();
}

void ScheduleTableModel::setDates(const QStringList& dates)
{
    m_dates = dates;
}

void ScheduleTableModel::setWorkers(const QStringList& workers)
{
    m_workers = workers;
}

void ScheduleTableModel::setSchedule(std::array<std::vector<std::vector<int>>, G::ShiftsCount> schedule)
{
    // slots / shifts / names
    m_schedule = schedule;

    QVector<QSet<qint32>> workers;
    workers.resize(3);

    // count workers per shift at total
    for (qint32 i = 0; i < G::ShiftsCount; ++i)
    {
        for (const auto& slot : schedule[i])
        {
            for (const auto& worker : slot)
            {
                workers[i].insert(worker);
            }
        }
    }

    for (qint32 i = 0; i < G::ShiftsCount; ++i)
    {
        m_workersCount[i] = workers.at(i).count();
    }

    m_rows    = std::accumulate(m_workersCount.begin(), m_workersCount.end(), 0) + 3; // [booking, names, residences, names, covid, names]
    m_columns = schedule.front().size() / 2 + 1;                                      // [names, dates]

    emit dataChanged(index(0, 0), index(m_rows - 1, m_columns - 1));

    emit headerDataChanged(Qt::Horizontal, 0, m_columns);
    emit headerDataChanged(Qt::Vertical, 0, m_rows);
}
