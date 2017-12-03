#include "highscoredialog.h"
#include "highscore_columns.h"
#include "utils.h"

#include <QHeaderView>

HighScoreDialog::HighScoreDialog(QSqlTableModel *model, QWidget *parent) : QDialog(parent)
{
    model->setHeaderData(as_integer(Columns::start), Qt::Horizontal, tr("Start date"));
    model->setHeaderData(as_integer(Columns::end), Qt::Horizontal, tr("End date"));
    model->setHeaderData(as_integer(Columns::startingPlayer), Qt::Horizontal, tr("Starting Player"));
    model->setHeaderData(as_integer(Columns::playerWon), Qt::Horizontal, tr("Player won"));
    model->setHeaderData(as_integer(Columns::score), Qt::Horizontal, tr("Score"));

    view = new QTableView;
    view->setModel(model);
    view->resizeColumnsToContents();
    view->verticalHeader()->hide();
    view->setSortingEnabled(true);
    view->sortByColumn(as_integer(Columns::start), Qt::SortOrder::DescendingOrder);
    view->setEditTriggers(QAbstractItemView::NoEditTriggers);
    view->setSelectionMode(QAbstractItemView::SingleSelection);
    view->setSelectionBehavior(QAbstractItemView::SelectRows);

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok);

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));

    mainLayout = new QVBoxLayout;
    mainLayout->addWidget(view);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);

    resize(680, 300);

    setWindowTitle(tr("High Score Dialog"));
}
