#include "clickablelabel.h"

ClickableLabel::ClickableLabel(QWidget *parent, Qt::WindowFlags flags)
{
    (void)parent;
    (void)flags;
}

void ClickableLabel::mousePressEvent(QMouseEvent *event)
{
    (void)event;
    emit clicked();
}
