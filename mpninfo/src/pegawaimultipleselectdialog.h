#ifndef PEGAWAIMULTIPLESELECTDIALOG_H
#define PEGAWAIMULTIPLESELECTDIALOG_H

#include <QDialog>

class QTableView;
class QComboBox;
class PegawaiMultipleSelectDialog : public QDialog
{
    Q_OBJECT
public:
    explicit
    PegawaiMultipleSelectDialog(QWidget *parent = 0);

    QString kodeKantor();
    int tahun();
    QStringList nipList();

private slots:
    void updateResult();
    void checkAll(bool toggle);

private:
    QTableView *mResultView;
    QComboBox *mKantorComboBox;
    QComboBox *mTahunComboBox;
};

#endif // PEGAWAIMULTIPLESELECTDIALOG_H
