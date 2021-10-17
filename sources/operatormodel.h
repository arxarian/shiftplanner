#pragma once

#include "operatoritem.h"

#include <QAbstractListModel>

class OperatorModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum Roles
    {
        ObjectRole = Qt::UserRole
    };
    Q_ENUM(Roles)

    explicit OperatorModel(QObject* parent = nullptr);

    Q_INVOKABLE virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    Q_INVOKABLE virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    Q_INVOKABLE virtual QHash<int, QByteArray> roleNames() const override;

private:
    QList<OperatorItem*> m_arrOperatorItems;
};
