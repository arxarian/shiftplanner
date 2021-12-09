#include "availabilitytablemodel.h"
#include "availabilityitem.h"
#include "availabilitymodel.h"

#include <QDebug>
#include <QPointer>

namespace AvailabilityTable
{
    const QString Residences("Pobyty");
    const QString Booking("Objednávání");
    const QString Covid("Covid");

    const QStringList Shifts{Residences, Booking, Covid};
} // namespace AvailabilityTable

AvailabilityTableModel::AvailabilityTableModel(QObject* parent) : QAbstractTableModel(parent)
{
    //
}

int AvailabilityTableModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)

    return m_workersNames.count();
}

int AvailabilityTableModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)

    return m_dates.count();
}

QVariant AvailabilityTableModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() >= rowCount(QModelIndex()) || index.column() >= columnCount(QModelIndex()))
    {
        return QVariant();
    }

    if (role == Qt::DisplayRole)
    {
        return m_workersAvailabitilty.value(m_workersNames.at(index.row())).at(index.column());
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
        }
        else
        {
            if (section < m_workersNames.count())
            {
                return m_workersNames.at(section);
            }
        }
    }
    return QVariant();
}

QStringList AvailabilityTableModel::workersNames() const
{
    return m_workersNames;
}

QStringList AvailabilityTableModel::dates() const
{
    return m_dates;
}

QMap<QString, QStringList> AvailabilityTableModel::workersAvailabitilty()
{
    return m_workersAvailabitilty;
}

void AvailabilityTableModel::setAvailabilityFromText(const QString& availabilityRaw)
{
    const QStringList&& rows = availabilityRaw.split(QChar::LineFeed);
    bool skipNextRow         = false;

    beginResetModel();

    for (const auto& row : rows)
    {
        if (skipNextRow)
        {
            skipNextRow = false;
            continue;
        }

        const QStringList& splittedRow = row.split(QChar::Tabulation);

        if (row.trimmed().isEmpty() || splittedRow.size() < 2)
        {
            break; // no more valid data
        }

        const QString& nameOrTypeColumn = splittedRow.at(1);

        if (nameOrTypeColumn.isEmpty())
        {
            continue;
        }

        if (AvailabilityTable::Shifts.contains(nameOrTypeColumn, Qt::CaseInsensitive))
        {
            skipNextRow = true;

            if (m_dates.isEmpty())
            {
                setDates(splittedRow);
            }
        }
        else
        {
            if (m_workersNames.contains(nameOrTypeColumn))
            {
                qDebug() << "woker" << nameOrTypeColumn << "already exists";
                qDebug() << splittedRow;
            }

            m_workersNames.append(nameOrTypeColumn);
            setWorkersAvailabitilty(splittedRow);
        }
    }

    endResetModel();

    qDebug() << "availability set";
}


void AvailabilityTableModel::setDates(const QStringList& splittedRow)
{
    QStringList t = splittedRow;
    t.removeAll("");
    m_dates = t.mid(1);

    emit headerDataChanged(Qt::Horizontal, 0, columnCount() - 1);
}

void AvailabilityTableModel::setWorkersAvailabitilty(const QStringList& splittedRow)
{
    m_workersAvailabitilty[splittedRow.at(1)] = splittedRow.mid(2, m_dates.count());
}
