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

class SkillHourTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit SkillHourTableModel(QObject* parent = nullptr);

    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    virtual int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex& index, int role) const override;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    QStringList workersNames() const;
    QMap<QString, Skills> workersSkills();
    QMap<QString, qint32> workersHours();

public slots:
    void setWorkersFromText(const QString& workersSkillsRaw, const bool project);

signals:
    void workersChanged(QStringList workers);

private:
    QStringList m_workersNames;

    QMap<QString, Skills> m_workersSkills;
    QMap<QString, qint32> m_workersHours;
};
