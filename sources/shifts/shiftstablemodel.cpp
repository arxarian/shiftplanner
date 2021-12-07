#include "shiftstablemodel.h"

#include "../global.h"

#include <QDebug>
#include <QPointer>

ShiftsTableModel::ShiftsTableModel(QObject* parent) : QAbstractTableModel(parent)
{
    //
}

int ShiftsTableModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)

    return G::DaysPerWeek * G::ShiftsCount + G::ShiftsCount; // + G::ShiftsCount because of descriptions
}

int ShiftsTableModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)

    return 4; // 1 per day name + 3 per min, opt and max
}

QVariant ShiftsTableModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() >= rowCount(QModelIndex()) || index.column() >= columnCount(QModelIndex()))
    {
        return QVariant();
    }

    if (role == Qt::DisplayRole)
    {
        const qint32 shiftIndex = index.row() / 6; // five days + description
        const qint32 dayIndex   = index.row() % 6;

        if (dayIndex == 0)
        {
            if (index.column() == 0)
            {
                return G::ShiftsNames.at(shiftIndex);
            }
            else
            {
                return G::ShiftsLimits.at(index.column() - 1);
            }
        }
        else
        {
            if (index.column() == 0)
            {
                return G::ShiftsDays.at(dayIndex - 1);
            }
            else
            {
                const Shift& shift = m_shifts.at(shiftIndex).at(dayIndex - 1);
                if (index.column() == 1)
                {
                    return shift.minimal;
                }
                else if (index.column() == 2)
                {
                    return shift.optimal;
                }
                else if (index.column() == 3)
                {
                    return shift.maximal;
                }
            }
        }
    }

    return QVariant();
}

QVariant ShiftsTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_UNUSED(orientation)

    if (role == Qt::DisplayRole)
    {
        return section;
    }

    return QVariant();
}

std::array<std::array<Shift, G::DaysPerWeek>, G::ShiftsCount> ShiftsTableModel::Shifts()
{
    return m_shifts;
}

void ShiftsTableModel::setShifts(const QString& shiftsRaw)
{
    QStringList&& rows = shiftsRaw.split(QChar::LineFeed);

    qint32 day = 0;

    for (const QString& row : rows)
    {
        if (row.isEmpty())
        {
            continue;
        }

        QStringList limits = row.split(QChar::Tabulation);
        qDebug() << limits;

        for (qint32 i = 1; i < limits.size(); i = i + 3) // skip day name
        {
            Shift& shift = m_shifts.at((i - 1) / 3).at(day);
            shift        = Shift{.minimal = limits.at(i).toInt(), .optimal = limits.at(i + 1).toInt(), .maximal = limits.at(i + 2).toInt()};
        }

        ++day;
    }

    emit headerDataChanged(Qt::Vertical, 0, rowCount() - 1);
    emit headerDataChanged(Qt::Horizontal, 0, columnCount() - 1);

    emit dataChanged(index(0, 0), index(rowCount() - 1, columnCount() - 1));
}
