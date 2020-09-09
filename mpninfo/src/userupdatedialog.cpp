#include "userupdatedialog.h"

#include <QDate>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QMessageBox>

#include "engine.h"
#include "cdata.h"
#include "pegawaiselectdialog.h"

UserUpdateDialog::UserUpdateDialog(QWidget *parent) :
    QDialog(parent, Qt::WindowTitleHint | Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint),
    mUserId(0)
{
    QLabel *fullnameLabel = new QLabel("Nama Lengkap");
    QLabel *usernameLabel = new QLabel("Username");
    QLabel *passwordLabel = new QLabel("Password");
    QLabel *rePasswordLabel = new QLabel("ReType");
    QLabel *userStatusLabel = new QLabel("Group");

    mUserFullnameEdit = new QLineEdit;
    mUsernameEdit = new QLineEdit;
    mPasswordEdit = new QLineEdit;
    mRePasswordEdit = new QLineEdit;
    mUserGroupCombo = new QComboBox;

    mPasswordEdit->setEchoMode(QLineEdit::Password);
    mRePasswordEdit->setEchoMode(QLineEdit::Password);

    mUserGroupCombo->addItem(tr("Users"), QVariant(GroupUser));

    mUserGroupCombo->setCurrentIndex(mUserGroupCombo->findData(GroupUser));

    mSelectButton = new QPushButton(tr("&Pilih"));
    QPushButton *okButton = new QPushButton(tr("&Ok"));
    QPushButton *cancelButton = new QPushButton(tr("&Batal"));

    mUserFullnameEdit->setEnabled(false);
    mUsernameEdit->setEnabled(false);

    QHBoxLayout *pegawaiBox = new QHBoxLayout;
    pegawaiBox->addWidget(mUserFullnameEdit);
    pegawaiBox->addWidget(mSelectButton);
    pegawaiBox->addStretch();

    QHBoxLayout *buttonBox = new QHBoxLayout;
    buttonBox->addStretch();
    buttonBox->addWidget(okButton);
    buttonBox->addWidget(cancelButton);

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(fullnameLabel, 0, 0, 1, 1);
    mainLayout->addWidget(usernameLabel, 1, 0, 1, 1);
    mainLayout->addWidget(passwordLabel, 2, 0, 1, 1);
    mainLayout->addWidget(rePasswordLabel, 2, 2, 1, 1);
    mainLayout->addWidget(userStatusLabel, 3, 0, 1, 1);
    mainLayout->addLayout(pegawaiBox, 0, 1, 1, 3);
    mainLayout->addWidget(mUsernameEdit, 1, 1, 1, 1);
    mainLayout->addWidget(mPasswordEdit, 2, 1, 1, 1);
    mainLayout->addWidget(mRePasswordEdit, 2, 3, 1, 1);
    mainLayout->addWidget(mUserGroupCombo, 3, 1, 1, 1);
    mainLayout->addLayout(buttonBox, 4, 0, 1, 4);

    setLayout(mainLayout);

    connect(okButton, SIGNAL(clicked()), SLOT(accept()));
    connect(cancelButton, SIGNAL(clicked()), SLOT(reject()));
    connect(mSelectButton, SIGNAL(clicked()), SLOT(selectPegawai()));
}

void UserUpdateDialog::setMode(int mode)
{
    if ( mode == AddMode ) {
        mSelectButton->setEnabled(true);
    }
    else {
        mSelectButton->setEnabled(false);
    }
}

void UserUpdateDialog::setUserId(int userId)
{
    Engine *engine = Engine::instance();
    User user = static_cast<CData *>(engine->data())->user(userId);

    mUserFullnameEdit->setText(user.fullname);
    mUsernameEdit->setText(user.username);
    mUserGroupCombo->setCurrentIndex(mUserGroupCombo->findData(user.group));
}

void UserUpdateDialog::selectPegawai()
{
    PegawaiSelectDialog dialog;
    dialog.setOnlyShowCurrentKantor(true);
    if ( dialog.exec() == QDialog::Accepted ) {
        Engine *engine = Engine::instance();
        Pegawai pegawai = engine->data()->pegawai(dialog.kodeKantor(), QDate::currentDate().year(), dialog.nip());

        mUserFullnameEdit->setText(pegawai.nama);
        mUsernameEdit->setText(pegawai.nip);
    }
}

QString UserUpdateDialog::fullname()
{
    return mUserFullnameEdit->text();
}

QString UserUpdateDialog::username()
{
    return mUsernameEdit->text();
}

QString UserUpdateDialog::password()
{
    return mPasswordEdit->text();
}

QString UserUpdateDialog::rePassword()
{
    return mRePasswordEdit->text();
}

int UserUpdateDialog::group()
{
    return mUserGroupCombo->itemData(mUserGroupCombo->currentIndex()).toInt();
}

