#ifndef USERSUPDATEDIALOG_H
#define USERSUPDATEDIALOG_H

#include <QDialog>

class QPushButton;
class QLineEdit;
class QComboBox;
class UserUpdateDialog : public QDialog
{
    Q_OBJECT
public:
    explicit UserUpdateDialog(QWidget *parent = 0);

    enum Mode {
        NoMode = 0,
        AddMode ,
        EditMode
    };

    void setUserId(int userId);
    void setMode(int mode);

    QString fullname();
    QString username();
    QString password();
    QString rePassword();
    int group();

private slots:
    void selectPegawai();

private:
    QPushButton *mSelectButton;
    QLineEdit *mUserFullnameEdit;
    QLineEdit *mUsernameEdit;
    QLineEdit *mPasswordEdit;
    QLineEdit *mRePasswordEdit;
    QComboBox *mUserGroupCombo;

    int mUserId;
    QString mPrevUsername;
    QString mPrevPassword;
    int mPrevGroup;
};

#endif // USERSUPDATEDIALOG_H
