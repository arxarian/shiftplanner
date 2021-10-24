#include "workeritem.h"

#include <QDebug>

WorkerItem::WorkerItem(QObject* parent) : QObject(parent)
{
    //
}

WorkerItem::WorkerItem(const QString& name, const qint32 maxHours, AvailabilityModel* availabilityModel, QObject* parent) :
  QObject(parent), m_name(name), m_maxHours(maxHours), m_availability(availabilityModel)
{
    if (availabilityModel)
    {
        availabilityModel->setParent(this);
    }

    qDebug() << "new worker" << name << availabilityModel->dump();
}

QString WorkerItem::name() const
{
    return m_name;
}

qint32 WorkerItem::maxHours() const
{
    return m_maxHours;
}

SkillModel* WorkerItem::skills() const
{
    return m_skills;
}

AvailabilityModel* WorkerItem::availability() const
{
    return m_availability;
}

void WorkerItem::setName(const QString& name)
{
    if (m_name == name)
        return;

    m_name = name;
    emit nameChanged(m_name);
}

void WorkerItem::setMaxHours(qint32 maxHours)
{
    if (m_maxHours == maxHours)
        return;

    m_maxHours = maxHours;
    emit maxHoursChanged(m_maxHours);
}

void WorkerItem::setSkills(SkillModel* skills)
{
    if (m_skills == skills)
        return;

    m_skills = skills;
    emit skillsChanged(m_skills);
}

void WorkerItem::setAvailability(AvailabilityModel* availability)
{
    if (m_availability == availability)
        return;

    m_availability = availability;
    emit availabilityChanged(m_availability);
}
