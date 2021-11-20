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
    m_skillsAndHoursModel = new SkillHourTableModel(this);

    connect(ui->toolButtonAvailabilityFrame, &QToolButton::clicked, this, [this] { ui->stackedWidget->setCurrentIndex(0); });
    connect(ui->toolButtonWorkersFrame, &QToolButton::clicked, this, [this] { ui->stackedWidget->setCurrentIndex(1); });
    connect(ui->toolButtonShiftsFrame, &QToolButton::clicked, this, [this] { ui->stackedWidget->setCurrentIndex(2); });

    connect(ui->pushButtonAddWorkers, &QPushButton::clicked, this, &MainWindow::AddAvailability);
    connect(ui->pushButtonAddSkills, &QPushButton::clicked, this, &MainWindow::AddSkillsAndHours);
    connect(ui->pushButtonGenerate, &QPushButton::clicked, this, &MainWindow::Plan);

    ui->tableViewAvailability->setModel(m_availabilityModel);
    ui->tableViewSkills->setModel(m_skillsAndHoursModel);

    AddAvailability();
    AddSkillsAndHours();

    m_planner->Plan(m_availabilityModel, m_skillsAndHoursModel);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::AddAvailability()
{
    // model must take the ownership

    const QString& text = QString("\t22. 10.\t23. 10.\t24. 10.\t25. 10.\t26. 10.\n"
                                  "Karel\tC\tC\tR\tX\tR\n"
                                  "Marek\tX\tX\tC\tR/O\tO\n"
                                  "Karolina\tX\tX\tX\tX\tR\n"
                                  "Lenka\tC\tC\tR\tX\tR\n"
                                  "Simona\tX\tX\tC\tR/O\tO\n"
                                  "Pavel\tX\tX\tX\tX\tR\n"
                                  "Cupito\tC\tC\tR\tX\tR\n"
                                  "Standa\tX\tX\tC\tR/O\tO\n"
                                  "Julie\tX\tX\tX\tX\tR\n"
                                  "Jan\tX\tX\tX\tX\tR\n"
                                  "Julie\tX\tX\tX\tX\tR\n"
                                  "Jan\tX\tX\tX\tX\tR\n");

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
    const QString& text = QString("\tSenior\tProject\tCovidt\tBooking\tResidences\tHours\n"
                                  "Karel\t\tNE\t\t\tX\t80\n"
                                  "Marek\tX\tANO\tX\tX\t\t80\n"
                                  "Karolina\tX\tANO\t\t\tX\t80\n"
                                  "Lenka\tX\tANO\t\t\tX\t80\n"
                                  "Simona\tX\tANO\t\t\tX\t80\n"
                                  "Pavel\tX\tANO\t\t\tX\t80\n"
                                  "Cupito\tX\tANO\t\t\tX\t80\n"
                                  "Standa\tX\tANO\t\t\tX\t80\n"
                                  "Julie\tX\tANO\t\t\tX\t80\n"
                                  "Jan\tX\tANO\t\t\tX\t80\n"
                                  "Julie\tX\tANO\t\t\tX\t80\n"
                                  "Jan\tX\tANO\t\t\tX\t80\n");

    QStringList&& rows = text.split(QChar::LineFeed);
    rows.removeFirst();

    m_skillsAndHoursModel->setWorkersSkillsAndHours(rows);

    //        ui->tableViewSkills->resizeColumnsToContents();
}

void MainWindow::Plan()
{
    m_planner->Plan(m_availabilityModel, m_skillsAndHoursModel);
}
