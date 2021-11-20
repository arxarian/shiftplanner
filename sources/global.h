#pragma once

#include <QtCore>

namespace G
{
    const QStringList SkillsNames = {"Senior", "Project", "Covid", "Booking", "Residences"};
    const QStringList ShiftsNames = {"Covid", "Booking", "Residences"};

    enum Shifts
    {
        Covid = 0,
        Booking,
        Residences
    };

} // namespace G
