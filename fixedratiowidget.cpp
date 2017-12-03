#include "fixedratiowidget.h"

void FixedRatioWidget::resizeEvent(QResizeEvent *event)
{
    if (width() > height())
    {
        resize(height(), height());
    }
    else
    {
        resize(width(), width());
    }
}
