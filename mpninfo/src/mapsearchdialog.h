#ifndef MAPSEARCHDIALOG_H
#define MAPSEARCHDIALOG_H

#include <QDialog>

class QLineEdit;
class QTableView;
class MapSearchDialog : public QDialog
{
    Q_OBJECT
public:
    explicit MapSearchDialog(QWidget *parent = nullptr);

private slots:
    void search();

private:
    QTableView *mResultView;
    QLineEdit *mKeywordEdit;
};

#endif // MAPSEARCHDIALOG_H
