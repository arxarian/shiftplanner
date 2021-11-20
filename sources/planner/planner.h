#pragma once

#include "sources/availability/availabilitytablemodel.h"
#include "sources/skills/skilltablemodel.h"

#include <QObject>

class Planner : public QObject
{
    Q_OBJECT

public:
    explicit Planner(QObject* parent = nullptr);

public slots:
    void Plan(AvailabilityTableModel* availabilityModel, SkillHourTableModel* skillHourModel);

signals:

private:
    AvailabilityTableModel* m_availabilityModel = nullptr;
    SkillHourTableModel* m_skillHourModel       = nullptr;

private:
    void ScheduleRequestsSat();
};
