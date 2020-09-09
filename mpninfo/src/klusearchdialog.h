#ifndef KLUSEARCHDIALOG_H
#define KLUSEARCHDIALOG_H

#include <QDialog>

class QLineEdit;
class QTreeView;
class KluSearchDialog : public QDialog
{
    Q_OBJECT
public:
    explicit KluSearchDialog(QWidget *parent = 0);

private slots:
    void search();

private:
    QTreeView *mResultView;
    QLineEdit *mKeywordEdit;
};

#endif // KLUSEARCHDIALOG_H
