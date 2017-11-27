#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include "gameprocess.h"
#include "gamepiece.h"
#include <QFrame>
#include <QSize>
#include <QEvent>
#include <QVector>

class GameBoard : public QFrame
{
    Q_OBJECT

public:
    GameBoard(QWidget *parent = 0);

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

public slots:
    void setState(GameProcess state);

signals:
    void stateChanged(GameProcess newState);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    const static int BOARD_SIZE = 7;

    QVector<GamePiece> *pieces;
};

#endif // GAMEBOARD_H
