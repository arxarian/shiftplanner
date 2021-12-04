#pragma once

#include "../global.h"

#include <QAbstractTableModel>

class ScheduleTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit ScheduleTableModel(QObject* parent = nullptr);

    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    virtual int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex& index, int role) const override;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

public slots:
    void setDates(const QStringList& dates);
    void setWorkers(const QStringList& workers);
    void setSchedule(std::array<std::vector<std::vector<int>>, G::ShiftsCount> schedule);

signals:
    void workersChanged(QStringList workers);

private:
    std::array<std::vector<std::vector<int>>, G::ShiftsCount> m_schedule;
    QVector<qint32> m_workersCount;
    QStringList m_workers;
    QStringList m_dates;
    qint32 m_rows    = 0;
    qint32 m_columns = 0;
};
