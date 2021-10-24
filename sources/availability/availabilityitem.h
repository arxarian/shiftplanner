#pragma once

#include <QDate>
#include <QObject>

class AvailabilityItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QDate day READ day WRITE setDay NOTIFY dayChanged)
    Q_PROPERTY(Availability availability READ availability WRITE setAvailability NOTIFY availabilityChanged)

public:
    enum Availability
    {
        Day,
        Morning,
        Afternoon,
        MorningOrAfterNoon,
        Unavailable
    };
    Q_ENUM(Availability)

    explicit AvailabilityItem(QObject* parent = nullptr);
    explicit AvailabilityItem(const QDate& day, Availability availability, QObject* parent = nullptr);
    explicit AvailabilityItem(const QDate& day, const QString& availability, QObject* parent = nullptr);

    const QString toString();

    QDate day() const;
    Availability availability() const;

public slots:
    void setDay(const QDate& day);
    void setAvailability(Availability availability);

signals:
    void dayChanged(QDate day);
    void availabilityChanged(Availability availability);

private:
    QDate m_day;
    Availability m_availability;
};
