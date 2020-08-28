#ifndef DATABASECONNECTIONDIALOG_H
#define DATABASECONNECTIONDIALOG_H

#include <QDialog>

class QSettings;
class QLabel;
class QComboBox;
class QLineEdit;
class Engine;
class DatabaseConnectionDialog : public QDialog
{
    Q_OBJECT
public:
    explicit DatabaseConnectionDialog(QWidget *parent = nullptr);

    int driver();
    QString driverName();
    QString hostname();
    QString username();
    QString password();
    QString database();
    QString options();

    void setDriver(int driver);

private slots:
    void updateWidget(int index);

private:
    QComboBox *mDriverCombo;
    QLineEdit *mDriverEdit;
    QLineEdit *mServerEdit;
    QLineEdit *mUsernameEdit;
    QLineEdit *mPasswordEdit;
    QLineEdit *mDatabaseEdit;
    QLineEdit *mOptionsEdit;
};


#endif // DATABASECONNECTIONDIALOG_H
