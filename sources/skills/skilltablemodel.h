#pragma once

#include "../global.h"

#include <QAbstractTableModel>

struct Skills
{
    enum SkillLevel
    {
        None = 0,
        Basic,
        Senior
    };

    std::array<SkillLevel, G::ShiftsCount> skills;

    bool ResidencesSkill() const;
    bool BookingSkill() const;
    bool CovidSkill() const;

    static SkillLevel TextToSkillLevel(const QString& levelRaw);
    static QString SkillLevelToText(const SkillLevel level);
};

struct WorkerSet
{
    bool m_project = false;
    Skills m_skills;
    qint32 m_hours = G::MaxHoursPerMonth;
};

class WorkersModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit WorkersModel(QObject* parent = nullptr);

    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    virtual int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex& index, int role) const override;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    QStringList workersNames() const;
    QMap<QString, WorkerSet> workers() const;

public slots:
    void setWorkersFromText(const QString& workersSkillsRaw, const bool project);

signals:
    void workersChanged(QStringList workers);

private:
    QStringList m_workersNames;
    QMap<QString, WorkerSet> m_workers;
};
