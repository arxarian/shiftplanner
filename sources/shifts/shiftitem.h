#pragma once

#include <QObject>

class ShiftItem : public QObject
{
    Q_OBJECT
    //    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    //    Q_PROPERTY(QDateTime start READ start WRITE setStart NOTIFY startChanged)
    //    Q_PROPERTY(QDateTime end READ end WRITE setEnd NOTIFY endChanged)
    // requirements - {[requirement, count], [requirement, count]}
    // spots
    // load - allocate more operators

public:
    explicit ShiftItem(QObject* parent = nullptr);

signals:
};
