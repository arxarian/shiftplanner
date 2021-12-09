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
    return QString("%1 %2").arg(splittedRow.at(TablePositions::Surname), splittedRow.at(TablePositions::Name));
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

WorkersModel::WorkersModel(QObject* parent) : QAbstractTableModel(parent)
{
    //
}

int WorkersModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)

    return m_workersNames.count();
}

int WorkersModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)

    return G::SkillsNames.count() + 2; // + 2 because of [name, residences, booking, covid, hours]
}

QVariant WorkersModel::data(const QModelIndex& index, int role) const
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
            const Skills& skills = m_workers.value(worker).m_skills;
            return Skills::SkillLevelToText(skills.skills.at(index.column() - 1)); // becuse in the first column, there is the name
        }
        else if (index.column() == G::SkillsNames.count() + 1)
        {
            return m_workers.value(worker).m_hours;
        }
    }

    return QVariant();
}

QVariant WorkersModel::headerData(int section, Qt::Orientation orientation, int role) const
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

QStringList WorkersModel::workersNames() const
{
    return m_workersNames;
}

QMap<QString, WorkerSet> WorkersModel::workers() const
{
    return m_workers;
}

void WorkersModel::setWorkersFromText(const QString& workersSkillsRaw, const bool project)
{
    QStringList&& rows = workersSkillsRaw.split(QChar::LineFeed);
    rows.removeFirst();

    beginResetModel();

    for (const QString& row : rows)
    {
        if (row.trimmed().isEmpty())
        {
            break; // no more valid data
        }

        const QStringList& splittedRow = row.split(QChar::Tabulation);
        const QString& workerName      = name(splittedRow);

        if (m_workersNames.contains(workerName))
        {
            qInfo() << "operator" << workerName << "exists";
            continue;
        }

        m_workersNames.append(workerName);

        m_workers[m_workersNames.last()].m_project = project;
        m_workers[m_workersNames.last()].m_skills  = Skills{.skills{
          residences(splittedRow),
          booking(splittedRow),
          covid(splittedRow),
        }};
        m_workers[m_workersNames.last()].m_hours   = G::MaxHoursPerMonth;
    }

    endResetModel();

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
