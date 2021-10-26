#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "sources/availability/availabilitytablemodel.h"
#include "sources/skills/skilltablemodel.h"

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void AddAvailability();
    void AddSkillsAndHours();

private:
    Ui::MainWindow* ui;

    AvailabilityTableModel* m_availabilityModel = nullptr;
    SkillHourTableModel* m_skillsAndHoursModel  = nullptr;

    //    QMap<QString, QStringList> m_workersAvailabitilty;
    //    QMap<QString, QSet<QString>> m_workersSkills;
    //    QMap<QString, qint32> m_workersHours;
};
#endif // MAINWINDOW_H
