#include "checkedheaderview.h"

#include <QPainter>
#include <QMouseEvent>

CheckedHeaderView::CheckedHeaderView(Qt::Orientation orientation, QWidget *parent) :
    QHeaderView(orientation, parent)
  , mChecked(false)
{
}

void CheckedHeaderView::setChecked(bool checked)
{
    mChecked = checked;
    updateSection(0);
    emit toggled(mChecked);
}

void CheckedHeaderView::paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const
{
    painter->save();
    QHeaderView::paintSection(painter, rect, logicalIndex);
    painter->restore();

    if (logicalIndex == 0) {
        QStyleOptionButton option;

        QRect checkboxRect = style()->subElementRect(QStyle::SE_CheckBoxIndicator, &option);
        option.rect = QRect(3, (rect.height()-checkboxRect.height()) / 2, checkboxRect.width(), checkboxRect.height());

        if (isEnabled())
            option.state |= QStyle::State_Enabled;

        if (mChecked)
            option.state |= QStyle::State_On;
        else
            option.state |= QStyle::State_Off;

        style()->drawControl(QStyle::CE_CheckBox, &option, painter);
    }
}

void CheckedHeaderView::mousePressEvent(QMouseEvent *event)
{
    if (isEnabled() && logicalIndexAt(event->pos()) == 0)
        setChecked(!mChecked);
    else
        QHeaderView::mousePressEvent(event);
}
