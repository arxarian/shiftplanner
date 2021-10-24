#pragma once

#include "workeritem.h"

#include <QAbstractListModel>

class WorkerModel : public QAbstractListModel
{
    Q_OBJECT
    //    Q_PROPERTY(qint32 name READ name WRITE setName NOTIFY nameChanged)

public:
    enum Roles
    {
        ObjectRole = Qt::UserRole
    };
    Q_ENUM(Roles)

    explicit WorkerModel(QObject* parent = nullptr);

    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    virtual QHash<int, QByteArray> roleNames() const override;

    void append(WorkerItem* item);

private:
    QList<WorkerItem*> m_items;
};
