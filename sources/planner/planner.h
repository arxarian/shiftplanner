#pragma once

#include "sources/availability/availabilitytablemodel.h"
#include "sources/skills/skilltablemodel.h"

#include "../global.h"

#include <QObject>

class Planner : public QObject
{
    Q_OBJECT

public:
    explicit Planner(QObject* parent = nullptr);

public slots:
    void Plan(AvailabilityTableModel* availabilityModel, SkillHourTableModel* skillHourModel);

signals:
    void Planned(std::vector<std::array<std::vector<int>, G::Shifts>> schedule);

private:
    AvailabilityTableModel* m_availabilityModel = nullptr;
    SkillHourTableModel* m_skillHourModel       = nullptr;

    std::vector<std::array<std::vector<int>, G::Shifts>> m_schedule; // slots, shifts, workers

private:
    void ScheduleRequestsSat();
};
