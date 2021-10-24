#pragma once

//#include "../workers/workeritem.h"
#include "availabilityitem.h"

#include <QAbstractListModel>

class AvailabilityModel : public QAbstractListModel
{
    Q_OBJECT
    //    Q_PROPERTY(WorkerItem* worker READ worker WRITE setWorker NOTIFY workerChanged)

public:
    enum Roles
    {
        ObjectRole = Qt::UserRole
    };
    Q_ENUM(Roles)

    explicit AvailabilityModel(QObject* parent = nullptr);
    explicit AvailabilityModel(const QList<QDate> dates, const QStringList& availabilities, QObject* parent = nullptr);

    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    virtual QHash<int, QByteArray> roleNames() const override;

    void append(AvailabilityItem* item);

    const QString dump() const;

private:
    QList<AvailabilityItem*> m_items;
};
