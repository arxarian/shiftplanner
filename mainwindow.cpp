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

    connect(ui->toolButtonAvailabilityFrame, &QToolButton::clicked, this, [this] { ui->stackedWidget->setCurrentIndex(0); });
    connect(ui->toolButtonWorkersFrame, &QToolButton::clicked, this, [this] { ui->stackedWidget->setCurrentIndex(1); });

    connect(ui->pushButtonAddWorkers, &QPushButton::clicked, this, &MainWindow::AddAvailability);
    connect(ui->pushButtonAddSkills, &QPushButton::clicked, this, &MainWindow::AddSkillsAndHours);

    m_availabilityModel = new AvailabilityTableModel(this);
    ui->tableViewAvailability->setModel(m_availabilityModel);

    m_skillsAndHoursModel = new SkillHourTableModel(this);
    ui->tableViewSkills->setModel(m_skillsAndHoursModel);

    AddAvailability();
    AddSkillsAndHours();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::AddAvailability()
{
    // model must take the ownership

    const QString& text =
      QString("\t22. 10.\t23. 10.\t24. 10.\t25. 10.\t26. 10.\nPavel\tC\tC\tR\tX\tR\nKarel\tX\tX\tC\tR/O\tO\nSimona\tX\tX\tX\tX\tR\n");

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
    const QString& text = QString("\tSenior\tProject\tCovidt\tReservations\tResidences\tHours"
                                  "\nPavel\tX\tANO\tX\tX\t\t80\nKarel\t\tNE\t\t\tX\t80\nSimona\tX\tANO\t\t\tX\t80\n");

    QStringList&& rows = text.split(QChar::LineFeed);
    rows.removeFirst();

    m_skillsAndHoursModel->setWorkersSkillsAndHours(rows);

    //        ui->tableViewSkills->resizeColumnsToContents();
}
