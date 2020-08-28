#include "databaseconnectiondialog.h"

#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QMessageBox>
#include <QFile>

#include "engine.h"

DatabaseConnectionDialog::DatabaseConnectionDialog(QWidget *parent) :
    QDialog(parent, Qt::WindowTitleHint | Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint)
{
    QLabel *iconLabel = new QLabel;

    mDriverCombo = new QComboBox;
    mDriverEdit = new QLineEdit;
    mServerEdit = new QLineEdit;
    mUsernameEdit = new QLineEdit;
    mPasswordEdit = new QLineEdit;
    mDatabaseEdit = new QLineEdit;
    mOptionsEdit = new QLineEdit;

    mOptionsEdit->setMinimumWidth(200);

    iconLabel->setPixmap(QPixmap(":/images/db.png"));
    iconLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    mDriverCombo->setMinimumWidth(150);
    mDriverCombo->addItem("MySQL / MariaDb", DatabaseMysql);
    mDriverCombo->addItem("SQLite", DatabaseSqlite);
    mDriverCombo->addItem("Other", DatabaseOther);
    mPasswordEdit->setEchoMode(QLineEdit::Password);

    mDriverEdit->setText("QMYSQL");
    mServerEdit->setText("127.0.0.1");
    mUsernameEdit->setText("root");
    mDatabaseEdit->setText("mpninfo");
    mOptionsEdit->setText("MYSQL_OPT_RECONNECT=1");
    mDriverEdit->setEnabled(false);

    QPushButton *okButton = new QPushButton("Simpan");

    QHBoxLayout *buttonBox = new QHBoxLayout;
    buttonBox->addStretch();
    buttonBox->addWidget(okButton);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(iconLabel, 0, 0, 2, 1, Qt::AlignTop);
    layout->addWidget(new QLabel("Driver"), 0, 1, 1, 1);
    layout->addWidget(mDriverCombo, 0, 2, 1, 1);
    layout->addWidget(new QLabel("Driver Name"), 1, 1, 1, 1);
    layout->addWidget(mDriverEdit, 1, 2, 1, 1);
    layout->addWidget(new QLabel("IP Address"), 2, 1, 1, 1);
    layout->addWidget(mServerEdit, 2, 2, 1, 1);
    layout->addWidget(new QLabel("Username"), 3, 1, 1, 1);
    layout->addWidget(mUsernameEdit, 3, 2, 1, 1);
    layout->addWidget(new QLabel("Password"), 4, 1, 1, 1);
    layout->addWidget(mPasswordEdit, 4, 2, 1, 1);
    layout->addWidget(new QLabel("Database"), 5, 1, 1, 1);
    layout->addWidget(mDatabaseEdit, 5, 2, 1, 1);
    layout->addWidget(new QLabel("Options"), 6, 1, 1, 1);
    layout->addWidget(mOptionsEdit, 6, 2, 1, 1);
    layout->addLayout(buttonBox, 7, 0, 1, 3);

    setLayout(layout);
    setWindowTitle("Setting Database");
    setWindowIcon(QIcon(":/images/settings.png"));

    connect(mDriverCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(updateWidget(int)));
    connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
}

void DatabaseConnectionDialog::setDriver(int driver)
{
    mDriverCombo->setCurrentIndex(mDriverCombo->findData(driver));
}

int DatabaseConnectionDialog::driver()
{
    return mDriverCombo->itemData(mDriverCombo->currentIndex()).toInt();
}

QString DatabaseConnectionDialog::driverName()
{
    return mDriverEdit->text();
}

QString DatabaseConnectionDialog::hostname()
{
    return mServerEdit->text();
}

QString DatabaseConnectionDialog::username()
{
    return mUsernameEdit->text();
}

QString DatabaseConnectionDialog::password()
{
    return mPasswordEdit->text();
}

QString DatabaseConnectionDialog::database()
{
    return mDatabaseEdit->text();
}

QString DatabaseConnectionDialog::options()
{
    return mOptionsEdit->text();
}

void DatabaseConnectionDialog::updateWidget(int index)
{
    int type = mDriverCombo->itemData(index).toInt();

    if (type == DatabaseMysql) {
        mDriverEdit->setText("QMYSQL");
        mServerEdit->setText("127.0.0.1");
        mUsernameEdit->setText("root");
        mPasswordEdit->setText("");
        mDatabaseEdit->setText("mpninfo");
        mOptionsEdit->setText("MYSQL_OPT_RECONNECT=1");

        mDriverEdit->setEnabled(false);
        mServerEdit->setEnabled(true);
        mUsernameEdit->setEnabled(true);
        mPasswordEdit->setEnabled(true);
        mDatabaseEdit->setEnabled(true);
        mOptionsEdit->setEnabled(true);
    }
    else if (type == DatabaseSqlite) {
        int result = QMessageBox::question(nullptr, "Encrypt Database", "Apakah Anda ingin mengencrypt database?", QMessageBox::Yes | QMessageBox::No);

        mDriverEdit->setText(result == QMessageBox::Yes? "QSQLCIPHER" : "QSQLITE");
        mServerEdit->setText("");
        mUsernameEdit->setText("");
        mPasswordEdit->setText("");
        mDatabaseEdit->setText("data.db");
        mOptionsEdit->setText("");

        mDriverEdit->setEnabled(false);
        mServerEdit->setEnabled(false);
        mUsernameEdit->setEnabled(false);
        mPasswordEdit->setEnabled(false);
        mDatabaseEdit->setEnabled(true);
        mOptionsEdit->setEnabled(false);
    }

    else if (type == DatabaseOther) {
        mDriverEdit->setText("");
        mServerEdit->setText("127.0.0.1");
        mUsernameEdit->setText("root");
        mPasswordEdit->setText("");
        mDatabaseEdit->setText("mpninfo");
        mOptionsEdit->setText("");

        mDriverEdit->setEnabled(true);
        mServerEdit->setEnabled(true);
        mUsernameEdit->setEnabled(true);
        mPasswordEdit->setEnabled(true);
        mDatabaseEdit->setEnabled(true);
        mOptionsEdit->setEnabled(true);
    }
}
