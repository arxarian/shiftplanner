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

    connect(ui->addOperatorsButton, &QToolButton::clicked, this, &MainWindow::AddOperators);

    m_workers = new WorkerModel(this);

    AddOperators(); // TODO - move as the last to test bindings

    AvailabilityTableModel* tableModel = new AvailabilityTableModel(this);
    tableModel->setWorkers(m_workers);
    ui->tableView->setModel(tableModel);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::AddOperators()
{
    // model must take the ownership

    const QString& text =
      QString("\t22. 10.\t23. 10.\t24. 10.\t25. 10.\t26. 10.\nPavel\tC\tC\tR\tX\tR\nKarel\tX\tX\tC\tR/O\tO\nSimona\tX\tX\tX\tX\tR\n");

    //    QClipboard* clipboard = QGuiApplication::clipboard();
    //    const QString& text   = clipboard->text();

    QStringList&& rows = text.split(QChar::LineFeed);
    //    rows.removeAt(1);  //
    rows.removeLast(); // it's an empty row anyway
    const QStringList& rawDates = rows.takeFirst().split(QChar::Tabulation);

    // extract dates
    QList<QDate> dates;
    for (const QString& rawDate : rawDates)
    {
        if (!rawDate.isEmpty())
        {
            const QStringList& dayAndMonth = rawDate.split(QChar('.'));
            dates.append(QDate(QDate::currentDate().year(), dayAndMonth.at(1).toInt(), dayAndMonth.at(0).toInt()));
        }
    }

    // create worker list
    for (const QString& row : rows)
    {
        AvailabilityModel* availabilities = new AvailabilityModel(dates, row.mid(row.indexOf(QChar::Tabulation) + 1).split(QChar::Tabulation));
        const QString& worker             = row.left(row.indexOf(QChar::Tabulation));
        m_workers->append(new WorkerItem(worker, 80, availabilities));
    }
}
