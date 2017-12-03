#ifndef HIGHSCOREDIALOG_H
#define HIGHSCOREDIALOG_H

#include <QWidget>
#include <QDialog>
#include <QTableView>
#include <QDialogButtonBox>
#include <QSqlTableModel>
#include <QPushButton>
#include <QLayout>

class HighScoreDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HighScoreDialog(QSqlTableModel *model, QWidget *parent = 0);

private:
    QTableView *view;
    QDialogButtonBox *buttonBox;
    QVBoxLayout *mainLayout;
};

#endif // HIGHSCOREDIALOG_H
