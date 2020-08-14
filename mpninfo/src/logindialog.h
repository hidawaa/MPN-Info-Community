#ifndef USERSLOGINDIALOG_H
#define USERSLOGINDIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
class QLineEdit;
QT_END_NAMESPACE

class LoginDialog : public QDialog
{
    Q_OBJECT
public:
    explicit LoginDialog(QWidget *parent = nullptr);

    QString username();
    QString password();
    void clear();

    void setUsername(const QString &username);
    void setPassword(const QString &password);

private:
    QLineEdit *mUsernameText;
    QLineEdit *mPasswordText;
    QPushButton *mOkButton;
    QPushButton *mQuitButton;

    QString mUsername;
};

#endif // USERSLOGINDIALOG_H
