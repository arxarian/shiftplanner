#pragma once

#include <QtCore>

namespace G
{
    const qint32 ShiftsCount = 3;
    const qint32 SlotsPerDay = 2;

    const QStringList SkillsNames = {"Senior", "Project", "Covid", "Booking", "Residences"};
    const QStringList ShiftsNames = {"Covid", "Booking", "Residences"};
    const QStringList ShiftsDays  = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday"};
    const QStringList PartsNames  = {"morning", "afternoon"};

    const QLatin1String DayShift("C");
    const QLatin1String MorningShift("R");
    const QLatin1String AfternoonShift("O");
    const QLatin1String MorningOrAfternoonShift("R/O");
    const QLatin1String NoShift("X");

    const QLatin1String Yes("ANO");
    const QLatin1String No("NE");

    enum ShiftTypes
    {
        Residences = 0,
        Booking,
        Covid
    };

    const qint32 Friday = 5;

    inline bool isSenior(const QStringList& skills) { return !skills.first().isEmpty(); } // the first skill is the senior skill
    inline bool isProject(const QStringList& skills) { return skills.at(1) == Yes; }
} // namespace G
