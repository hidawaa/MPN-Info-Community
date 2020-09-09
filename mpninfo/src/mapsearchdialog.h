#ifndef MAPSEARCHDIALOG_H
#define MAPSEARCHDIALOG_H

#include <QDialog>

class QLineEdit;
class QTreeView;
class MapSearchDialog : public QDialog
{
    Q_OBJECT
public:
    explicit MapSearchDialog(QWidget *parent = nullptr);

private slots:
    void search();

private:
    QTreeView *mResultView;
    QLineEdit *mKeywordEdit;
};

#endif // MAPSEARCHDIALOG_H
