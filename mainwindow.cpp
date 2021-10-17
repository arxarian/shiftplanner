#include "mainwindow.h"
#include "sources/operatoritem.h"
#include "sources/operatormodel.h"
#include "sources/skillitem.h"
#include "sources/skillmodel.h"
#include "ui_mainwindow.h"

#include <QClipboard>
#include <QDebug>

QLatin1String COVID("COVID");
QLatin1String RESIDENCES("RESIDENCES");
QLatin1String RESERVATIONS("RESERVATIONS");
QLatin1String SENIOR("SENIOR");
QLatin1String PROJECT("PROJECT");
QLatin1String NON_PROJECT("NON_PROJECT");


MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->addOperatorsButton, &QToolButton::clicked, this, &MainWindow::AddOperators);

    // model must take the ownership
    OperatorItem* operatorItem = new OperatorItem("Karl", 80, new SkillModel(QSet<QString>{COVID, PROJECT, SENIOR}));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::AddOperators()
{
    QClipboard* clipboard = QGuiApplication::clipboard();
    QString originalText  = clipboard->text();
    qDebug() << "***" << originalText;
}
