#include "skilltablemodel.h"

#include <QDebug>
#include <QPointer>

const QStringList Skills = {"Senior", "Project", "Covid", "Reservations", "Residences"};

SkillHourTableModel::SkillHourTableModel(QObject* parent) : QAbstractTableModel(parent)
{
    //
}

int SkillHourTableModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)

    return m_workers.count();
}

int SkillHourTableModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)

    return Skills.count() + 1;
}

QVariant SkillHourTableModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() >= rowCount(QModelIndex()) || index.column() >= columnCount(QModelIndex()))
    {
        return QVariant();
    }

    if (role == Qt::DisplayRole)
    {
        const QString& worker = m_workers.at(index.row());
        if (index.column() < Skills.count())
        {
            return m_workersSkills.value(worker).at(index.column());
        }
        else if (index.column() == Skills.count())
        {
            return m_workersHours.value(worker);
        }
    }

    return QVariant();
}

QVariant SkillHourTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal)
        {
            if (section < Skills.count())
            {
                return Skills.value(section);
            }
            else if (section == Skills.count())
            {
                return "Hours";
            }
        }
        else if (orientation == Qt::Vertical)
        {
            if (section < m_workers.count())
            {
                return m_workers.at(section);
            }
        }
    }
    return QVariant();
}

QStringList SkillHourTableModel::workers() const
{
    return m_workers;
}

QMap<QString, QStringList> SkillHourTableModel::workersSkills()
{
    return m_workersSkills;
}

QMap<QString, qint32> SkillHourTableModel::workersHours()
{
    return m_workersHours;
}

void SkillHourTableModel::setWorkersSkillsAndHours(const QStringList& workersSkills)
{
    m_workers.clear();
    m_workersSkills.clear();
    m_workersHours.clear();

    for (const QString& row : workersSkills)
    {
        if (row.isEmpty())
        {
            continue;
        }

        QStringList workerSkills = row.split(QChar::Tabulation);
        m_workers.append(workerSkills.takeFirst());

        m_workersHours[m_workers.last()]  = workerSkills.takeLast().toInt();
        m_workersSkills[m_workers.last()] = workerSkills;
    }

    emit headerDataChanged(Qt::Vertical, 0, m_workers.count() - 1);
    emit dataChanged(index(0, 0), index(m_workers.count() - 1, Skills.count())); // +1 because of hours column
    emit workersChanged(m_workers);
}
