#ifndef USERSMANAGEDIALOG_H
#define USERSMANAGEDIALOG_H

#include <QDialog>
#include <QModelIndex>

class QTableView;
class UserManageDialog : public QDialog
{
    Q_OBJECT
public:
    UserManageDialog(QWidget *parent = 0);

    void reset();

private slots:
    void addUser();
    void editUser();
    void deleteUser();

private slots:
    void updateModel();
    void updateButton();

private:
    QTableView *mUsersView;
    QPushButton *mAddButton;
    QPushButton *mDelButton;
    QPushButton *mEditButton;

    int mSelectedUserId;
    int mSelectedUserGroup;

};


#endif // USERSMANAGEDIALOG_H
