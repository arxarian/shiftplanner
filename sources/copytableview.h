#pragma once

#include <QTableView>

class CopyTableView : public QTableView
{
    Q_OBJECT
public:
    CopyTableView(QWidget* parent = 0);

public slots:
    void keyPressEvent(QKeyEvent* event);
};
