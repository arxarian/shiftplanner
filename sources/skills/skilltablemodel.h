#pragma once

#include <QAbstractTableModel>

class SkillHourTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit SkillHourTableModel(QObject* parent = nullptr);

    virtual int rowCount(const QModelIndex& parent) const override;
    virtual int columnCount(const QModelIndex& parent) const override;
    virtual QVariant data(const QModelIndex& index, int role) const override;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    QStringList workers() const;
    QMap<QString, QStringList> workersSkills();
    QMap<QString, qint32> workersHours();

public slots:
    void setWorkersSkillsAndHours(const QStringList& workersSkills);

signals:
    void workersChanged(QStringList workers);

private:
    QStringList m_workers;

    QMap<QString, QStringList> m_workersSkills;
    QMap<QString, qint32> m_workersHours;
};
