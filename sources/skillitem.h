#pragma once

#include <QObject>

class SkillItem : public QObject
{
    Q_OBJECT
    // int id?
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)

public:
    explicit SkillItem(QObject* parent = nullptr);

signals:
};
