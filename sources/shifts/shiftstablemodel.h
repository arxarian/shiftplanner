#pragma once

#include "../global.h"

#include <QAbstractTableModel>

struct Shift
{
    qint32 minimal = 0;
    qint32 optimal = 0;
    qint32 maximal = 0;
};

class ShiftsTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit ShiftsTableModel(QObject* parent = nullptr);

    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    virtual int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex& index, int role) const override;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    std::array<std::array<Shift, G::DaysPerWeek>, G::ShiftsCount> Shifts();

public slots:
    void setShifts(const QString& shiftsRaw);

signals:

private:
    std::array<std::array<Shift, G::DaysPerWeek>, G::ShiftsCount> m_shifts; // [covid, residences, booking][monday - friday][min, opt, max]
};
