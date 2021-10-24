#include "availabilityitem.h"

#include <QMap>

const QMap<QString, AvailabilityItem::Availability> TEXT_TO_AVAILAVILITY = {{"C", AvailabilityItem::Day},
                                                                            {"R", AvailabilityItem::Morning},
                                                                            {"O", AvailabilityItem::Afternoon},
                                                                            {"R/O", AvailabilityItem::MorningOrAfterNoon},
                                                                            {"X", AvailabilityItem::Unavailable}};

AvailabilityItem::AvailabilityItem(QObject* parent) : QObject(parent)
{
    //
}

AvailabilityItem::AvailabilityItem(const QDate& day, AvailabilityItem::Availability availability, QObject* parent) :
  QObject(parent), m_day(day), m_availability(availability)
{
    //
}

AvailabilityItem::AvailabilityItem(const QDate& day, const QString& availability, QObject* parent) : QObject(parent), m_day(day)
{
    m_availability = TEXT_TO_AVAILAVILITY.value(availability);
}

const QString AvailabilityItem::toString()
{
    return TEXT_TO_AVAILAVILITY.key(m_availability);
}

QDate AvailabilityItem::day() const
{
    return m_day;
}

AvailabilityItem::Availability AvailabilityItem::availability() const
{
    return m_availability;
}

void AvailabilityItem::setDay(const QDate& day)
{
    if (m_day == day)
        return;

    m_day = day;
    emit dayChanged(m_day);
}

void AvailabilityItem::setAvailability(AvailabilityItem::Availability availability)
{
    if (m_availability == availability)
        return;

    m_availability = availability;
    emit availabilityChanged(m_availability);
}
