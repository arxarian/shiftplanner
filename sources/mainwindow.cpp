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

    m_planner            = new Planner(this);
    m_availabilityModel  = new AvailabilityTableModel(this);
    m_scheduleTableModel = new ScheduleTableModel(this);
    m_workersModel       = new WorkersModel(this);
    m_shiftsTableModel   = new ShiftsTableModel(this);

    connect(m_planner, &Planner::Planned, m_scheduleTableModel, &ScheduleTableModel::setSchedule);

    connect(ui->toolButtonAvailabilityFrame, &QToolButton::clicked, this, [this] { ui->stackedWidget->setCurrentIndex(0); });
    connect(ui->toolButtonWorkersFrame, &QToolButton::clicked, this, [this] { ui->stackedWidget->setCurrentIndex(1); });
    connect(ui->toolButtonShiftsFrame, &QToolButton::clicked, this, [this] { ui->stackedWidget->setCurrentIndex(2); });
    connect(ui->toolButtonScheduleFrame, &QToolButton::clicked, this, [this] { ui->stackedWidget->setCurrentIndex(3); });

    connect(ui->pushButtonAddAvailability, &QPushButton::clicked, this, &MainWindow::AddAvailability);
    connect(ui->pushButtonAddProjectWorkers, &QPushButton::clicked, this, &MainWindow::AddProjectWorkers);
    connect(ui->pushButtonAddNonProjectWorkers, &QPushButton::clicked, this, &MainWindow::AddNonProjectWorkers);
    connect(ui->pushButtonGenerate, &QPushButton::clicked, this, &MainWindow::Plan);

    QSortFilterProxyModel* projectWorkersFilter = new QSortFilterProxyModel(this);
    projectWorkersFilter->setSourceModel(m_workersModel);

    QSortFilterProxyModel* nonProjectWorkersFilter = new QSortFilterProxyModel(this);
    nonProjectWorkersFilter->setSourceModel(m_workersModel);

    ui->tableViewAvailability->setModel(m_availabilityModel);
    ui->tableViewProjectWorkers->setModel(projectWorkersFilter);
    ui->tableViewNonProjectWorkers->setModel(nonProjectWorkersFilter);
    ui->tableViewRegularShifts->setModel(m_shiftsTableModel);
    ui->tableViewSchedule->setModel(m_scheduleTableModel);

    ui->toolButtonWorkersFrame->click();

    const QDate& date = QDate::currentDate();
    ui->dateEditFrom->setDate(date.addDays(-date.day() + 1));
    ui->dateEditTo->setDate(date.addDays(-date.day() + date.daysInMonth()));

    AddShifts();
    AddProjectWorkers();
    AddNonProjectWorkers();
    AddAvailability();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::AddAvailability()
{
    //    QClipboard* clipboard = QGuiApplication::clipboard();
    //    const QString& text   = clipboard->text();

    QString text;

    QFile f("availabilities_november.txt");
    if (f.open(QFile::ReadOnly | QFile::Text))
    {
        QTextStream in(&f);
        text = in.readAll();
    }

    m_availabilityModel->setAvailabilityFromText(text);

    //    ui->tableViewAvailability->resizeColumnsToContents();

    m_scheduleTableModel->setWorkers(m_availabilityModel->workersNames());
    m_scheduleTableModel->setDates(m_availabilityModel->dates());
}

void MainWindow::AddProjectWorkers()
{
    //    QClipboard* clipboard = QGuiApplication::clipboard();
    //    const QString& text   = clipboard->text();

    QString text;

    QFile f("project_workers.txt");
    if (f.open(QFile::ReadOnly | QFile::Text))
    {
        QTextStream in(&f);
        text = in.readAll();
    }

    m_workersModel->setWorkersFromText(text, true);

    ui->tableViewProjectWorkers->resizeColumnsToContents();
}

void MainWindow::AddNonProjectWorkers()
{
    //    QClipboard* clipboard = QGuiApplication::clipboard();
    //    const QString& text   = clipboard->text();

    QString text;

    QFile f("non_project_workers.txt");
    if (f.open(QFile::ReadOnly | QFile::Text))
    {
        QTextStream in(&f);
        text = in.readAll();
    }

    m_workersModel->setWorkersFromText(text, false);

    ui->tableViewProjectWorkers->resizeColumnsToContents();
}

void MainWindow::AddShifts()
{
    const QString& text = QString("Monday\t15\t15\t15\t9\t11\t11\t2\t2\t2\n"
                                  "Tuesday\t15\t15\t16\t8\t9\t9\t2\t2\t3\n"
                                  "Wednesday\t15\t15\t16\t8\t9\t9\t2\t2\t3\n"
                                  "Thursday\t12\t12\t12\t7\t7\t8\t1\t1\t2\n"
                                  "Friday\t11\t11\t11\t6\t6\t7\t1\t1\t2\n");

    m_shiftsTableModel->setShiftsFromText(text);
}

void MainWindow::Plan()
{
    m_planner->Plan(m_availabilityModel, m_workersModel, m_shiftsTableModel);
}
