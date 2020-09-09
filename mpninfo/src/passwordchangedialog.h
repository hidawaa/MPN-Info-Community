#ifndef PASSWORDCHANGEDIALOG_H
#define PASSWORDCHANGEDIALOG_H

#include <QDialog>

class QLineEdit;
class PasswordChangeDialog : public QDialog
{
    Q_OBJECT
public:
    explicit PasswordChangeDialog(QWidget *parent = 0);

private slots:
    void ubah();

private:
    QLineEdit *mPrevPasswordEdit;
    QLineEdit *mPasswordEdit;
    QLineEdit *mRepasswordEdit;
};

#endif // PASSWORDCHANGEDIALOG_H
