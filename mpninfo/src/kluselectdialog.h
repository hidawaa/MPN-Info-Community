#ifndef KLUSELECTDIALOG_H
#define KLUSELECTDIALOG_H

#include <QDialog>

class QLineEdit;
class QTreeView;
class KluSelectDialog : public QDialog
{
    Q_OBJECT
public:
    explicit KluSelectDialog(QWidget *parent = 0);

    QString klu();

private slots:
    void search();

private:
    QTreeView *mResultView;
    QLineEdit *mKeywordEdit;
};

#endif // KLUSELECTDIALOG_H
