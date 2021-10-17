#include "operatoritem.h"

OperatorItem::OperatorItem(QObject* parent) : QObject(parent)
{
    //
}

OperatorItem::OperatorItem(const QString& name, const qint32 maxHours, SkillModel* skillModel, QObject* parent) : QObject(parent)
{
    skillModel->setParent(this);
}
