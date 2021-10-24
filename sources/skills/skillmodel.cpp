#include "skillmodel.h"

#include <QPointer>
#include <QSet>

SkillModel::SkillModel(QObject* parent) : QAbstractListModel(parent)
{
    //
}

SkillModel::SkillModel(QSet<SkillItem::Skill> skills, QObject* parent)
{
    //
}

int SkillModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)

    return m_arrSkillItems.size();
}

QVariant SkillModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() >= rowCount())
    {
        return QVariant();
    }

    QPointer<SkillItem> pSkillItem = m_arrSkillItems.at(index.row());
    if (role == Roles::ObjectRole)
    {
        return QVariant::fromValue(pSkillItem.data());
    }

    return QVariant();
}

QHash<int, QByteArray> SkillModel::roleNames() const
{
    return {{SkillModel::Roles::ObjectRole, "object"}};
}
