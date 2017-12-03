#ifndef CLICKABLELABEL_H
#define CLICKABLELABEL_H

#include <QWidget>
#include <QLabel>

class ClickableLabel : public QLabel
{
    Q_OBJECT

public:
    explicit ClickableLabel(QWidget *parent = 0, Qt::WindowFlags flags = 0);

signals:
    void clicked();

protected:
    void mousePressEvent(QMouseEvent *event);
};

#endif // CLICKABLELABEL_H
