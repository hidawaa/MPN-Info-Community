#ifndef DATABASESETTINGDIALOG_H
#define DATABASESETTINGDIALOG_H

#include <QDialog>

class QSettings;
class QLabel;
class QComboBox;
class QLineEdit;
class Engine;
class DatabaseSettingDialog : public QDialog
{
    Q_OBJECT
public:
    explicit DatabaseSettingDialog(QWidget *parent = nullptr);

    int driver();
    QString hostname();
    QString username();
    QString password();
    QString database();

    void setDriver(int driver);

private slots:
    void updateWidget(int index);

private:
    QLabel *mServerLabel;
    QLabel *mUsernameLabel;
    QLabel *mPasswordLabel;
    QLabel *mDatabaseLabel;

    QComboBox *mDriverCombo;
    QLineEdit *mServerEdit;
    QLineEdit *mUsernameEdit;
    QLineEdit *mPasswordEdit;
    QLineEdit *mDatabaseEdit;
};


#endif // DATABASESETTINGDIALOG_H
