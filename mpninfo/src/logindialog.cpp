#include "logindialog.h"

#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QMessageBox>
#include <QSqlDatabase>

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent, Qt::WindowTitleHint | Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint)
{
    QLabel *usernameLabel = new QLabel("NIP (9 digit)");
    QLabel *passwordLabel = new QLabel("Password");
    QLabel *iconLabel = new QLabel;
    iconLabel->setPixmap(QPixmap(":/images/auth.png"));

    mUsernameText = new QLineEdit;
    mUsernameText->setToolTip("Username");
    mUsernameText->setFocus(Qt::OtherFocusReason);
    mUsernameText->setMinimumWidth(150);
    mPasswordText = new QLineEdit;
    mPasswordText->setToolTip("Password");
    mPasswordText->setEchoMode(QLineEdit::Password);
    mPasswordText->setMinimumWidth(150);

    mOkButton = new QPushButton("&Login");

    QHBoxLayout *buttonBox = new QHBoxLayout;
    buttonBox->addStretch();
    buttonBox->addWidget(mOkButton);

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(iconLabel, 0, 0, 2, 1, Qt::AlignTop);
    mainLayout->addWidget(usernameLabel, 0, 1, 1, 1);
    mainLayout->addWidget(mUsernameText, 0, 2, 1, 1);
    mainLayout->addWidget(passwordLabel, 1, 1, 1, 1);
    mainLayout->addWidget(mPasswordText, 1, 2, 1, 1);
    mainLayout->addLayout(buttonBox, 2, 0, 1, 3);

    setLayout(mainLayout);
    setWindowTitle("Login");
    setWindowIcon(QIcon(":/images/logoff.png"));

    connect(mOkButton, SIGNAL(clicked()), this, SLOT(accept()));
}

void LoginDialog::clear()
{
    mPasswordText->clear();
    mUsernameText->setFocus();
}

void LoginDialog::setUsername(const QString &username)
{
    mUsernameText->setText(username);
    mPasswordText->setFocus();
}

void LoginDialog::setPassword(const QString &password)
{
    mPasswordText->setText(password);
}

QString LoginDialog::username()
{
    return mUsernameText->text();
}

QString LoginDialog::password()
{
    return mPasswordText->text();
}
