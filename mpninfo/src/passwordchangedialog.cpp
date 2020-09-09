#include "passwordchangedialog.h"

#include <QSqlQuery>
#include <QLabel>
#include <QIcon>
#include <QPushButton>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QMessageBox>
#include <QCryptographicHash>

#include "engine.h"

PasswordChangeDialog::PasswordChangeDialog(QWidget *parent) :
    QDialog(parent, Qt::WindowTitleHint | Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint)
{
    QLabel *oldPassLabel = new QLabel("Password Lama");
    QLabel *newPassLabel = new QLabel("Password Baru");
    QLabel *rePassLabel = new QLabel("ReType Password");
    QLabel *iconLabel = new QLabel;

    iconLabel->setPixmap(QPixmap(":/images/auth.png"));

    mPrevPasswordEdit = new QLineEdit;
    mPasswordEdit = new QLineEdit;
    mRepasswordEdit = new QLineEdit;

    mPrevPasswordEdit->setEchoMode(QLineEdit::Password);
    mPasswordEdit->setEchoMode(QLineEdit::Password);
    mRepasswordEdit->setEchoMode(QLineEdit::Password);

    QPushButton *okButton = new QPushButton("Ubah");
    QPushButton *cancelButton = new QPushButton("Batal");

    connect(okButton, SIGNAL(clicked()), this, SLOT(ubah()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));

    QHBoxLayout *buttonBox = new QHBoxLayout;
    buttonBox->addStretch();
    buttonBox->addWidget(okButton);
    buttonBox->addWidget(cancelButton);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(iconLabel, 0,0,3,1, Qt::AlignTop);
    layout->addWidget(oldPassLabel, 0,1,1,1);
    layout->addWidget(newPassLabel, 1,1,1,1);
    layout->addWidget(rePassLabel, 2,1,1,1);
    layout->addWidget(mPrevPasswordEdit, 0,2,1,1);
    layout->addWidget(mPasswordEdit, 1,2,1,1);
    layout->addWidget(mRepasswordEdit, 2,2,1,1);
    layout->addLayout(buttonBox, 3, 0, 1, 3);

    setLayout(layout);
    setWindowTitle("Ubah Password");
    setWindowIcon(QIcon(tr(":/images/auth.png")));
}

void PasswordChangeDialog::ubah()
{
    Engine *engine = Engine::instance();
    QString username = engine->user().username;

    if ( mPrevPasswordEdit->text().isEmpty()
         || mPasswordEdit->text().isEmpty()
         || mRepasswordEdit->text().isEmpty() ) {
        QMessageBox::information(this, "Info", "Isian tidak boleh kosong.");
        return;
    }

    DatabasePtr db = engine->database();
    db->exec("SELECT `value` FROM `info` WHERE `key`='db.version'");

    QString version;
    if (db->next())
        version = db->value(0).toString();

    QString selectSql = QString("SELECT `password` FROM `users` WHERE `username`='%1'").arg(username);
    db->exec(selectSql);

    QString auth;
    if (db->next()) {
        QString password = db->value(0).toString();
        if (version  < "5.0")
            auth = mPrevPasswordEdit->text();
        else
            auth = QCryptographicHash::hash(QString(username + ":" + mPrevPasswordEdit->text()).toUtf8(), QCryptographicHash::Sha1).toHex();

        if (password != auth) {
            QMessageBox::warning(this, "Error", "Password lama salah.");
            mPrevPasswordEdit->clear();
            return;
        }
    }

    if (mPasswordEdit->text().length() < 5 ) {
        QMessageBox::information(this, "Info", "Panjang password minimal 5 karakter.");
        mPasswordEdit->clear();
        mRepasswordEdit->clear();
        return;
    }

    if (mPasswordEdit->text() != mRepasswordEdit->text() ) {
        QMessageBox::information(this, "Info", "Password baru tidak sama.");
        mPasswordEdit->clear();
        mRepasswordEdit->clear();
        return;
    }

    if (version  < "5.0")
        auth = mPasswordEdit->text();
    else
        auth = QCryptographicHash::hash(QString(username + ":" + mPasswordEdit->text()).toUtf8(), QCryptographicHash::Sha1).toHex();

    db->exec(QString("UPDATE `users` SET `password`='%1' WHERE `username`='%2'").arg(auth, username));
    if (db->numRowsAffected() > 0) {
        QMessageBox::information(this, "Berhasil", "Password berhasil diubah.");
        accept();
    }
    else {
        QMessageBox::warning(this, "Gagal", "Password gagal diubah.");
        reject();
    }
}
