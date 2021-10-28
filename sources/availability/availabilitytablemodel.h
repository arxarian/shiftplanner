#pragma once

#include "../workers/workeritem.h"
#include "../workers/workermodel.h"

#include <QAbstractTableModel>

class AvailabilityTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit AvailabilityTableModel(QObject* parent = nullptr);

    virtual int rowCount(const QModelIndex& parent) const override;
    virtual int columnCount(const QModelIndex& parent) const override;
    virtual QVariant data(const QModelIndex& index, int role) const override;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    QStringList workers() const;
    QStringList dates() const;
    QMap<QString, QStringList> workersAvailabitilty();

public slots:
    void setDates(const QStringList& dates);
    void setWorkersAvailabitilty(const QStringList& workersAvailabitilty);

signals:
    void workersChanged(QStringList workers);

private:
    QStringList m_dates;
    QStringList m_workers;

    QMap<QString, QStringList> m_workersAvailabitilty;
};
