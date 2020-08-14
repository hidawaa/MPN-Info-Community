#include "databasesettingdialog.h"

#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QMessageBox>
#include <QFile>

#include "engine.h"

DatabaseSettingDialog::DatabaseSettingDialog(QWidget *parent) :
    QDialog(parent, Qt::WindowTitleHint | Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint)
{
    QLabel *typeLabel = new QLabel("Driver");
    QLabel *iconLabel = new QLabel;
    mServerLabel = new QLabel("IP Address");
    mUsernameLabel = new QLabel("Username");
    mPasswordLabel = new QLabel("Password");
    mDatabaseLabel = new QLabel("Database");

    mDriverCombo = new QComboBox;
    mServerEdit = new QLineEdit;
    mUsernameEdit = new QLineEdit;
    mPasswordEdit = new QLineEdit;
    mDatabaseEdit = new QLineEdit;

    iconLabel->setPixmap(QPixmap(":/images/db.png"));
    iconLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    mDriverCombo->setMinimumWidth(150);
    mDriverCombo->addItem("MySQL / MariaDb", DatabaseMysql);
    mDriverCombo->addItem("SQLite", DatabaseSqlite);
    mPasswordEdit->setEchoMode(QLineEdit::Password);

    mServerEdit->setText("127.0.0.1");
    mDatabaseEdit->setText("mpninfo");

    QPushButton *okButton = new QPushButton("Simpan");

    QHBoxLayout *buttonBox = new QHBoxLayout;
    buttonBox->addStretch();
    buttonBox->addWidget(okButton);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(iconLabel, 0, 0, 2, 1, Qt::AlignTop);
    layout->addWidget(typeLabel, 0, 1, 1, 1);
    layout->addWidget(mDriverCombo, 0, 2, 1, 1);
    layout->addWidget(mServerLabel, 1, 1, 1, 1);
    layout->addWidget(mServerEdit, 1, 2, 1, 1);
    layout->addWidget(mUsernameLabel, 2, 1, 1, 1);
    layout->addWidget(mUsernameEdit, 2, 2, 1, 1);
    layout->addWidget(mPasswordLabel, 3, 1, 1, 1);
    layout->addWidget(mPasswordEdit, 3, 2, 1, 1);
    layout->addWidget(mDatabaseLabel, 4, 1, 1, 1);
    layout->addWidget(mDatabaseEdit, 4, 2, 1, 1);
    layout->addLayout(buttonBox, 5, 0, 1, 3);

    setLayout(layout);
    setWindowTitle("Setting Database");
    setWindowIcon(QIcon(":/images/settings.png"));

    connect(mDriverCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(updateWidget(int)));
    connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
}

void DatabaseSettingDialog::setDriver(int driver)
{
    mDriverCombo->setCurrentIndex(mDriverCombo->findData(driver));
}

int DatabaseSettingDialog::driver()
{
    return mDriverCombo->itemData(mDriverCombo->currentIndex()).toInt();
}

QString DatabaseSettingDialog::hostname()
{
    return mServerEdit->text();
}

QString DatabaseSettingDialog::username()
{
    return mUsernameEdit->text();
}

QString DatabaseSettingDialog::password()
{
    return mPasswordEdit->text();
}

QString DatabaseSettingDialog::database()
{
    return mDatabaseEdit->text();
}

void DatabaseSettingDialog::updateWidget(int index)
{
    if (mDriverCombo->itemData(index).toInt() == DatabaseMysql) {
        mServerEdit->setEnabled(true);
        mUsernameEdit->setEnabled(true);
        mPasswordEdit->setEnabled(true);
        mDatabaseEdit->setEnabled(true);
    }
    else {
        mServerEdit->setEnabled(false);
        mUsernameEdit->setEnabled(false);
        mPasswordEdit->setEnabled(false);
        mDatabaseEdit->setEnabled(false);
    }
}
