#include "skilltablemodel.h"

#include "../global.h"

#include <QDebug>
#include <QPointer>

const QLatin1String SeniorText = QLatin1String("senior");
const QLatin1String BasicText  = QLatin1String("ano");
const QLatin1String NoneText   = QLatin1String("ne");

namespace TablePositions
{
    const qint32 Surname         = 1;
    const qint32 Name            = 2;
    const qint32 BookingSkill    = 9;
    const qint32 ResidencesSkill = 10;
    const qint32 CovidSkill      = 11;
} // namespace TablePositions

QString name(const QStringList& splittedRow)
{
    return QString("%1 %2").arg(splittedRow.at(TablePositions::Surname)).arg(splittedRow.at(TablePositions::Name));
}

Skills::SkillLevel booking(const QStringList& splittedRow)
{
    return Skills::TextToSkillLevel(splittedRow.at(TablePositions::BookingSkill));
}

Skills::SkillLevel residences(const QStringList& splittedRow)
{
    return Skills::TextToSkillLevel(splittedRow.at(TablePositions::ResidencesSkill));
}

Skills::SkillLevel covid(const QStringList& splittedRow)
{
    return Skills::TextToSkillLevel(splittedRow.at(TablePositions::CovidSkill));
}

SkillHourTableModel::SkillHourTableModel(QObject* parent) : QAbstractTableModel(parent)
{
    //
}

int SkillHourTableModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)

    return m_workersNames.count();
}

int SkillHourTableModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)

    return G::SkillsNames.count() + 2; // + 2 because of [name, residences, booking, covid, hours]
}

QVariant SkillHourTableModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() >= rowCount(QModelIndex()) || index.column() >= columnCount(QModelIndex()))
    {
        return QVariant();
    }

    if (role == Qt::DisplayRole)
    {
        const QString& worker = m_workersNames.at(index.row());
        if (index.column() == 0)
        {
            return worker;
        }
        else if (index.column() < G::SkillsNames.count() + 1) // becuse in the first column, there is the name
        {
            const Skills& skills = m_workersSkills.value(worker);
            return Skills::SkillLevelToText(skills.skills.at(index.column() - 1)); // becuse in the first column, there is the name
        }
        else if (index.column() == G::SkillsNames.count() + 1)
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
            if (section == 0)
            {
                return "Name";
            }
            else if (section < G::SkillsNames.count() + 1)
            {
                return G::SkillsNames.value(section - 1);
            }
            else if (section == G::SkillsNames.count() + 1)
            {
                return "Hours";
            }
        }
        else if (orientation == Qt::Vertical)
        {
            return section;
        }
    }
    return QVariant();
}

QStringList SkillHourTableModel::workersNames() const
{
    return m_workersNames;
}

QMap<QString, Skills> SkillHourTableModel::workersSkills()
{
    return m_workersSkills;
}

QMap<QString, qint32> SkillHourTableModel::workersHours()
{
    return m_workersHours;
}

void SkillHourTableModel::setWorkersFromText(const QString& workersSkillsRaw, const bool project)
{
    QStringList&& rows = workersSkillsRaw.split(QChar::LineFeed);
    rows.removeFirst();

    m_workersNames.clear();
    m_workersSkills.clear();
    m_workersHours.clear();

    for (const QString& row : rows)
    {
        if (row.trimmed().isEmpty())
        {
            break; // no more valid data
        }

        const QStringList& splittedRow = row.split(QChar::Tabulation);
        m_workersNames.append(name(splittedRow));

        m_workersHours[m_workersNames.last()]  = G::MaxHoursPerMonth;
        m_workersSkills[m_workersNames.last()] = Skills{.skills{
          residences(splittedRow),
          booking(splittedRow),
          covid(splittedRow),
        }};
    }

    emit headerDataChanged(Qt::Vertical, 0, columnCount() - 1);
    emit dataChanged(index(0, 0), index(rowCount() - 1, columnCount() - 1));
    emit workersChanged(m_workersNames);
}

bool Skills::ResidencesSkill() const
{
    return skills.at(G::ShiftTypes::Residences) > SkillLevel::None;
}

bool Skills::BookingSkill() const
{
    return skills.at(G::ShiftTypes::Booking) > SkillLevel::None;
}

bool Skills::CovidSkill() const
{
    return skills.at(G::ShiftTypes::Covid) > SkillLevel::None;
}

Skills::SkillLevel Skills::TextToSkillLevel(const QString& levelRaw)
{
    SkillLevel level = None;

    if (levelRaw == BasicText)
    {
        level = Basic;
    }
    else if (levelRaw == SeniorText)
    {
        level = Senior;
    }

    return level;
}

QString Skills::SkillLevelToText(const SkillLevel level)
{
    QString rawLevel = NoneText;

    if (level == Basic)
    {
        rawLevel = BasicText;
    }
    else if (level == Senior)
    {
        rawLevel = SeniorText;
    }

    return rawLevel;
}
