#include "copytableview.h"

#include <QApplication>
#include <QClipboard>
#include <QKeyEvent>

CopyTableView::CopyTableView(QWidget* parent) : QTableView(parent) {}

void CopyTableView::keyPressEvent(QKeyEvent* event)
{
    if (event->matches(QKeySequence::Copy))
    {
        QModelIndexList cells = selectedIndexes();
        std::sort(cells.begin(), cells.end()); // Necessary, otherwise they are in column order

        QString text;
        qint32 currentRow = 0; // To determine when to insert newlines
        for (const QModelIndex& cell : cells)
        {
            if (text.length() == 0)
            {
                // First item
            }
            else if (cell.row() != currentRow)
            {
                // New row
                text += '\n';
            }
            else
            {
                // Next cell
                text += '\t';
            }
            currentRow = cell.row();
            text += cell.data().toString();
        }

        QApplication::clipboard()->setText(text);
    }
    else if (event->matches(QKeySequence::SelectAll))
    {
        selectAll();
    }
}
