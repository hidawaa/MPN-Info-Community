#ifndef PEGAWAIUPDATEDIALOG_H
#define PEGAWAIUPDATEDIALOG_H

#include <QDialog>

class QLineEdit;
class QComboBox;
class PegawaiUpdateDialog : public QDialog
{
    Q_OBJECT
public:
    explicit PegawaiUpdateDialog(QWidget *parent = 0);

    enum Mode {
        AddMode = 0,
        EditMode
    };

    void setKantor(const QString kodeKantor);
    void setNipPegawai(const QString &kodeKantor, const QString &nip, int tahun);
    void setMode(int mode);

    QString nip();
    QString nip2();
    QString nama();
    int seksi();
    int pangkat();
    int jabatan();
    QString plh();

private slots:
    void selectPlh();
    void removePlh();

private:
    QLineEdit *mNipEdit;
    QLineEdit *mNip2Edit;
    QLineEdit *mNamaEdit;
    QComboBox *mSeksiCombo;
    QComboBox *mPangkatCombo;
    QComboBox *mJabatanCombo;
    QLineEdit *mPlhEdit;

    QString mKodeKantor;
};


#endif // PEGAWAIUPDATEDIALOG_H
