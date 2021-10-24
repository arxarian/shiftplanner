#pragma once

#include <QObject>

class SkillItem : public QObject
{
    Q_OBJECT
    // int id?
    //    Q_PROPERTY(Skill skill READ skill WRITE setSkill NOTIFY skillChanged)

public:
    enum Skill
    {
        Senior,
        Project,
        Non_Project,
        Covid,
        Reservations,
        Residences
    };

    explicit SkillItem(QObject* parent = nullptr);

signals:
};
