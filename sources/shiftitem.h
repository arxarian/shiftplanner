#pragma once

#include <QObject>

class OperatorItem : public QObject
{
    Q_OBJECT
    // int id?
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(qint32 maxHours READ maxHours WRITE setMaxHours NOTIFY maxHoursChanged)
    Q_PROPERTY(SkillModel* skills READ skills WRITE setSkills NOTIFY skillsChanged)
    // availability

public:
    explicit OperatorItem(QObject* parent = nullptr);
    explicit OperatorItem(const QString& name, const qint32 maxHours, SkillModel* skillModel, QObject* parent = nullptr);

signals:
};
