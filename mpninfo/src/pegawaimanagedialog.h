#ifndef PEGAWAIMANAGEDIALOG_H
#define PEGAWAIMANAGEDIALOG_H

#include <QDialog>
#include <QModelIndex>

class QTableView;
class QComboBox;
class PegawaiManageDialog : public QDialog
{
    Q_OBJECT
public:
    explicit PegawaiManageDialog(QWidget *parent = 0);

    void reset();

private slots:
    void addPegawai();
    void editPegawai();
    void deletePegawai();
    void copyPegawai();

private slots:
    void updateModel();
    void updateButton();

private:
    QTableView *mPegawaiView;

    QComboBox *mKantorComboBox;
    QComboBox *mTahunComboBox;

    QPushButton *mOkButton;
    QPushButton *mAddButton;
    QPushButton *mDelButton;
    QPushButton *mEditButton;

    QString mSelectedPegawaiNip;
};

#endif // PEGAWAIMANAGEDIALOG_H
