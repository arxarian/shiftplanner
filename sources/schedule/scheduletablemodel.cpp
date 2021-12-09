#include "scheduletablemodel.h"

#include "../global.h"

#include <QDebug>
#include <QFont>
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
        qint32 shift = 0;
        qint32 row   = index.row();

        for (; shift < G::ShiftsCount; ++shift)
        {
            if (row - m_workersCount[shift] > 0)
            {
                row = row - m_workersCount[shift] - 1;
            }
            else
            {
                break;
            }
        }

        bool description = false;
        if (row == 0)
        {
            description = true;
        }

        --row;

        if (index.column() == 0 && description)
        {
            // shifts names
            return G::ShiftsNames.at(shift);
        }
        else if (index.column() == columnCount() - 1 && description)
        {
            return "Hours";
        }
        else if (index.column() > 0 && description)
        {
            // dates
            return m_dates.at(index.column() - 1);
        }
        else if (index.column() == 0)
        {
            // workers names
            return m_workersNames.at(m_workersIdsPerShift.at(shift).at(row));
        }
        else if (index.column() == columnCount() - 1)
        {
            const qint32 workerId                      = m_workersIdsPerShift.at(shift).at(row);
            const std::vector<std::vector<int>> _slots = m_schedule.at(shift);

            // shifts / slots / names
            // TODO - redo, very expensive
            qint32 hours = 0;

            for (const auto& shift : m_schedule)
            {
                for (const auto& _slot : shift)
                {
                    for (const auto& worker : _slot)
                    {
                        if (worker == workerId)
                        {
                            hours += 4;
                        }
                    }
                }
            }

            return hours;
        }
        else
        {
            // shifts
            // shifts / slots / names

            const qint32 workerId                      = m_workersIdsPerShift.at(shift).at(row);
            const std::vector<std::vector<int>> _slots = m_schedule.at(shift);

            const qint32 slot = (index.column() - 1) * 2;

            const bool morning  = std::find(_slots.at(slot).begin(), _slots.at(slot).end(), workerId) != _slots.at(slot).end();
            const bool afternon = std::find(_slots.at(slot + 1).begin(), _slots.at(slot + 1).end(), workerId) != _slots.at(slot + 1).end();

            if (morning && afternon)
            {
                return G::DayShift;
            }
            else if (morning)
            {
                return G::MorningShift;
            }
            else if (afternon)
            {
                return G::AfternoonShift;
            }
        }
    }
    //    else if (role == Qt::FontRole && index.column() == 0)
    //    { // First column items are bold.
    //        QFont font;
    //        font.setBold(true);
    //        return font;
    //    }

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
    m_workersNames = workers;
}

void ScheduleTableModel::setSchedule(std::array<std::vector<std::vector<int>>, G::ShiftsCount> schedule)
{
    // shifts / slots / names
    m_schedule = schedule;
    QVector<QSet<qint32>> workersIdsPerShift;
    workersIdsPerShift.resize(G::ShiftsCount);

    // count workers per shift at total
    for (qint32 i = 0; i < G::ShiftsCount; ++i)
    {
        for (const auto& slot : schedule[i])
        {
            for (const auto& worker : slot)
            {
                workersIdsPerShift[i].insert(worker);
            }
        }
    }

    m_workersIdsPerShift.clear();
    m_workersIdsPerShift.resize(G::ShiftsCount);
    for (qint32 i = 0; i < G::ShiftsCount; ++i)
    {
        m_workersCount[i] = workersIdsPerShift.at(i).count();
        m_workersIdsPerShift[i].append(workersIdsPerShift.at(i).values());
    }

    m_rows    = std::accumulate(m_workersCount.begin(), m_workersCount.end(), 0) + 3; // [booking, names, residences, names, covid, names]
    m_columns = schedule.front().size() / 2 + 1 + 1;                                  // [names, dates, total hours]

    emit dataChanged(index(0, 0), index(m_rows - 1, m_columns - 1));

    emit headerDataChanged(Qt::Horizontal, 0, m_columns);
    emit headerDataChanged(Qt::Vertical, 0, m_rows);
}
