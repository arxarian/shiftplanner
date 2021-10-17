#pragma once

#include "skillitem.h"

#include <QAbstractListModel>

class SkillModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum Roles
    {
        ObjectRole = Qt::UserRole
    };
    Q_ENUM(Roles)

    explicit SkillModel(QObject* parent = nullptr);
    explicit SkillModel(QSet<QString> skills, QObject* parent = nullptr);

    Q_INVOKABLE virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    Q_INVOKABLE virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    Q_INVOKABLE virtual QHash<int, QByteArray> roleNames() const override;

private:
    QList<SkillItem*> m_arrSkillItems;
};
