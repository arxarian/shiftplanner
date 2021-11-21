#pragma once

#include <QtCore>

namespace G
{
    const QStringList SkillsNames = {"Senior", "Project", "Covid", "Booking", "Residences"};
    const QStringList ShiftsNames = {"Covid", "Booking", "Residences"};
    const QStringList PartsNames  = {"morning", "afternoon"};

    const QLatin1String DayShift("C");
    const QLatin1String MorningShift("R");
    const QLatin1String AfternoonShift("O");
    const QLatin1String MorningOrAfternoonShift("R/O");
    const QLatin1String NoShift("X");

    enum Shifts
    {
        Covid = 0,
        Booking,
        Residences
    };
} // namespace G
