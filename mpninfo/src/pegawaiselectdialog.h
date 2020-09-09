#ifndef PEGAWAISELECTDIALOG_H
#define PEGAWAISELECTDIALOG_H

#include <QDialog>

class QTableView;
class QComboBox;
class PegawaiSelectDialog : public QDialog
{
    Q_OBJECT
public:
    explicit PegawaiSelectDialog(QWidget *parent = nullptr);

    QString kodeKantor();
    inline QString nip() { return mSelectedPegawaiNip; }

    void setOnlyShowCurrentKantor(bool value);
    void setOnlyShowKantor(const QString &kodeKpp);
    void setFilterByJabatan(int jabatan);

private slots:
    void updateModel();
    void updateButton();

private:
    QString mSelectedPegawaiNip;

    QComboBox *mKantorComboBox;

    QTableView *mPegawaiView;
    QPushButton *mOkButton;
    int mJabatan;
};


#endif // PEGAWAISELECTDIALOG_H
