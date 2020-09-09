#ifndef CHECKEDHEADERVIEW_H
#define CHECKEDHEADERVIEW_H

#include <QHeaderView>

class CheckedHeaderView : public QHeaderView
{
    Q_OBJECT
public:
    explicit CheckedHeaderView(Qt::Orientation orientation, QWidget *parent = 0);
    
    void setChecked(bool checked);
    inline bool checked() { return mChecked; }

signals:
    void toggled(bool checked);

protected:
    void paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const;
    void mousePressEvent(QMouseEvent *event);

private:
    bool mChecked;
};

#endif // CHECKEDHEADERVIEW_H
