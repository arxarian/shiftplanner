#include "mainwindow.h"
#include "sources/availability/availabilitymodel.h"
#include "sources/availability/availabilitytablemodel.h"
#include "sources/skills/skillitem.h"
#include "sources/skills/skillmodel.h"
#include "sources/workers/workeritem.h"
#include "ui_mainwindow.h"

#include <QClipboard>
#include <QDebug>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_planner             = new Planner(this);
    m_availabilityModel   = new AvailabilityTableModel(this);
    m_scheduleTableModel  = new ScheduleTableModel(this);
    m_skillsAndHoursModel = new SkillHourTableModel(this);
    m_shiftsTableModel    = new ShiftsTableModel(this);

    connect(m_planner, &Planner::Planned, m_scheduleTableModel, &ScheduleTableModel::setSchedule);

    connect(ui->toolButtonAvailabilityFrame, &QToolButton::clicked, this, [this] { ui->stackedWidget->setCurrentIndex(0); });
    connect(ui->toolButtonWorkersFrame, &QToolButton::clicked, this, [this] { ui->stackedWidget->setCurrentIndex(1); });
    connect(ui->toolButtonShiftsFrame, &QToolButton::clicked, this, [this] { ui->stackedWidget->setCurrentIndex(2); });
    connect(ui->toolButtonScheduleFrame, &QToolButton::clicked, this, [this] { ui->stackedWidget->setCurrentIndex(3); });

    connect(ui->pushButtonAddWorkers, &QPushButton::clicked, this, &MainWindow::AddAvailability);
    connect(ui->pushButtonAddSkills, &QPushButton::clicked, this, &MainWindow::AddSkillsAndHours);
    connect(ui->pushButtonGenerate, &QPushButton::clicked, this, &MainWindow::Plan);

    ui->tableViewAvailability->setModel(m_availabilityModel);
    ui->tableViewSkills->setModel(m_skillsAndHoursModel);
    ui->tableViewRegularShifts->setModel(m_shiftsTableModel);
    ui->tableViewSchedule->setModel(m_scheduleTableModel);

    ui->toolButtonScheduleFrame->click();

    const QDate& date = QDate::currentDate();
    ui->dateEditFrom->setDate(date.addDays(-date.day() + 1));
    ui->dateEditTo->setDate(date.addDays(-date.day() + date.daysInMonth()));

    AddAvailability();
    AddSkillsAndHours();
    AddShifts();

    m_scheduleTableModel->setWorkers(m_availabilityModel->workers());
    m_scheduleTableModel->setDates(m_availabilityModel->dates());

    m_planner->Plan(m_availabilityModel, m_skillsAndHoursModel);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::AddAvailability()
{
    // model must take the ownership

    const QString& text = QString("\t25. 10.\t26. 10.\t27. 10.\t28. 10.\t29. 10.\n"
                                  "Karel\tR/O\tR/O\tR/O\tR/O\tR/O\n"
                                  "Marek\tX\tC\tC\tR/O\tO\n"
                                  "Karolina\tC\tO\tX\tR\tR\n"
                                  "Lenka\tC\tC\tR\tX\tR\n"
                                  "Simona\tC\tC\tC\tR/O\tO\n"
                                  "Pavel\tR\tR\tR\tR\tR\n"
                                  "Cupito\tC\tC\tR\tX\tR\n"
                                  "Standa\tR/O\tR/O\tR/O\tR/O\tO\n"
                                  "Julie\tC\tC\tC\tC\tC\n"
                                  "Jan\tC\tO\tR\tC\tR\n"
                                  "Lenka\tC\tC\tC\tR\tC\n"
                                  "Jan\tR/O\tR/O\tR/O\tR/O\tR\n"
                                  "Superman\tC\tC\tC\tC\tC\n"
                                  "Lois\tC\tC\tC\tC\tC\n");

    //    QClipboard* clipboard = QGuiApplication::clipboard();
    //    const QString& text   = clipboard->text();

    QStringList&& rows         = text.split(QChar::LineFeed);
    QString&& horizontalHeader = rows.takeFirst();
    //    rows.removeFirst(); // useless data

    QStringList&& dates = horizontalHeader.split(QChar::Tabulation);
    dates.removeFirst();
    m_availabilityModel->setDates(dates);

    m_availabilityModel->setWorkersAvailabitilty(rows);

    ui->tableViewAvailability->resizeColumnsToContents();
}

void MainWindow::AddSkillsAndHours()
{
    const QString& text = QString("\tSenior\tProject\tResidencest\tBooking\tCovid\tHours\n"
                                  "Karel\tX\tNE\t\t\tX\t80\n"
                                  "Marek\t\tANO\tX\tX\t\t80\n"
                                  "Karolina\t\tANO\tX\tX\tX\t80\n"
                                  "Lenka\tX\tANO\t\tX\tX\t80\n"
                                  "Simona\tX\tNE\tX\t\tX\t80\n"
                                  "Pavel\tX\tNE\t\tX\tX\t80\n"
                                  "Cupito\tX\tANO\t\tX\tX\t20\n"
                                  "Standa\t\tANO\t\t\tX\t80\n"
                                  "Julie\tX\tNE\tX\tX\t\t80\n"
                                  "Jan\tX\tANO\tX\tX\t\t80\n"
                                  "Lenka\tX\tNE\tX\tX\t\t80\n"
                                  "Jan\t\tANO\tX\tX\tX\t80\n"
                                  "Superman\tX\tANO\tX\tX\tX\t80\n"
                                  "Lois\tX\tNE\tX\tX\tX\t80\n");

    QStringList&& rows = text.split(QChar::LineFeed);
    rows.removeFirst();

    m_skillsAndHoursModel->setWorkersSkillsAndHours(rows);

    //        ui->tableViewSkills->resizeColumnsToContents();
}

void MainWindow::AddShifts()
{
    const QString& text = QString("Monday\t15\t15\t15\t9\t11\t11\t2\t2\t2\n"
                                  "Tuesday\t15\t15\t16\t8\t9\t9\t2\t2\t3\n"
                                  "Wednesday\t15\t15\t16\t8\t9\t9\t2\t2\t3\n"
                                  "Thursday\t12\t12\t12\t7\t7\t8\t1\t1\t2\n"
                                  "Friday\t11\t11\t11\t6\t6\t7\t1\t1\t2\n");

    m_shiftsTableModel->setShifts(text);
}

void MainWindow::Plan()
{
    m_planner->Plan(m_availabilityModel, m_skillsAndHoursModel);
}
