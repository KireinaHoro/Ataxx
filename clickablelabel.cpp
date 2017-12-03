#include "clickablelabel.h"

ClickableLabel::ClickableLabel(QWidget *parent, Qt::WindowFlags flags)
{

}

void ClickableLabel::mousePressEvent(QMouseEvent *event)
{
    emit clicked();
}
