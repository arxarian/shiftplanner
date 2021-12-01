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
    const QString& text = QString("\tSenior\tProject\tCovidt\tBooking\tResidences\tHours\n"
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

void MainWindow::Plan()
{
    m_planner->Plan(m_availabilityModel, m_skillsAndHoursModel);
}
