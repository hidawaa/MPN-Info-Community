#ifndef SEKSIUPDATEDIALOG_H
#define SEKSIUPDATEDIALOG_H

#include <QDialog>

class QLineEdit;
class QComboBox;
class SeksiUpdateDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SeksiUpdateDialog(QWidget *parent = 0);

    void setSeksi(const QString &kantor, int id);

    int seksi();
    QString name();
    QString telepon();
    QString kode();

private slots:
    void checkValues();
    void seksiSelected();

private:
    QComboBox *mSeksiComboBox;
    QLineEdit *mNameEdit;
    QLineEdit *mTelpEdit;
    QLineEdit *mKodeEdit;
};

#endif // SEKSIUPDATEDIALOG_H
