#ifndef FIXEDRATIOWIDGET_H
#define FIXEDRATIOWIDGET_H

#include <QWidget>

class FixedRatioWidget : public QWidget
{
    Q_OBJECT

protected:
    void resizeEvent(QResizeEvent *event);
};

#endif // FIXEDRATIOWIDGET_H
