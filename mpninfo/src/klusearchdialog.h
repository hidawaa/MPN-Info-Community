#ifndef KLUSEARCHDIALOG_H
#define KLUSEARCHDIALOG_H

#include <QDialog>

class QLineEdit;
class QTableView;
class KluSearchDialog : public QDialog
{
    Q_OBJECT
public:
    explicit KluSearchDialog(QWidget *parent = 0);

private slots:
    void search();

private:
    QTableView *mResultView;
    QLineEdit *mKeywordEdit;
};

#endif // KLUSEARCHDIALOG_H
