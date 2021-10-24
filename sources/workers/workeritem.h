#pragma once

#include "../availability/availabilitymodel.h"
#include "../skills/skillmodel.h"

#include <QObject>

class WorkerItem : public QObject
{
    Q_OBJECT
    // int id?
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(qint32 maxHours READ maxHours WRITE setMaxHours NOTIFY maxHoursChanged)
    Q_PROPERTY(SkillModel* skills READ skills WRITE setSkills NOTIFY skillsChanged)
    Q_PROPERTY(AvailabilityModel* availability READ availability WRITE setAvailability NOTIFY availabilityChanged)

public:
    explicit WorkerItem(QObject* parent = nullptr);
    explicit WorkerItem(const QString& name, const qint32 maxHours = 0, AvailabilityModel* availabilityModel = nullptr, QObject* parent = nullptr);

    QString name() const;
    qint32 maxHours() const;
    SkillModel* skills() const;
    AvailabilityModel* availability() const;

public slots:
    void setName(const QString& name);
    void setMaxHours(qint32 maxHours);
    void setSkills(SkillModel* skills);
    void setAvailability(AvailabilityModel* availability);

signals:
    void nameChanged(QString name);
    void maxHoursChanged(qint32 maxHours);
    void skillsChanged(SkillModel* skills);
    void availabilityChanged(AvailabilityModel* availability);

private:
    QString m_name;
    qint32 m_maxHours;
    SkillModel* m_skills              = nullptr;
    AvailabilityModel* m_availability = nullptr;
};
