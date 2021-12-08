#pragma once

#include "sources/availability/availabilitytablemodel.h"
#include "sources/shifts/shiftstablemodel.h"
#include "sources/skills/skilltablemodel.h"

#include "../global.h"

#include <QObject>

class Planner : public QObject
{
    Q_OBJECT

public:
    explicit Planner(QObject* parent = nullptr);

public slots:
    void Plan(AvailabilityTableModel* availabilityModel, SkillHourTableModel* skillHourModel, ShiftsTableModel* shiftsTableModel);

signals:
    void Planned(std::array<std::vector<std::vector<int>>, G::ShiftsCount> schedule);

private:
    AvailabilityTableModel* m_availabilityModel = nullptr;
    SkillHourTableModel* m_skillHourModel       = nullptr;
    ShiftsTableModel* m_shiftsTableModel        = nullptr;

    std::array<std::vector<std::vector<int>>, G::ShiftsCount> m_schedule; // shifts, slots, workers

private:
    void ScheduleRequestsSat();
};
