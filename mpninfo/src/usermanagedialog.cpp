#include "usermanagedialog.h"

#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTableView>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QMessageBox>
#include <QStandardItemModel>

#include "engine.h"
#include "cdata.h"
#include "userupdatedialog.h"

UserManageDialog::UserManageDialog(QWidget *parent) :
    QDialog(parent, Qt::WindowTitleHint | Qt::WindowCloseButtonHint),
    mSelectedUserId(0), mSelectedUserGroup(0)
{
    mUsersView = new QTableView;
    mUsersView->setAlternatingRowColors(true);
    mUsersView->setShowGrid(false);
    mUsersView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mUsersView->setSelectionBehavior(QAbstractItemView::SelectRows);
    mUsersView->setSelectionMode(QAbstractItemView::SingleSelection);
    mUsersView->horizontalHeader()->setStretchLastSection(true);
    mUsersView->horizontalHeader()->setHighlightSections(false);
    mUsersView->verticalHeader()->hide();
    mUsersView->verticalHeader()->setDefaultSectionSize(fontMetrics().height()+8);

    QPushButton *okButton = new QPushButton(tr("&Ok"));
    mAddButton = new QPushButton(tr("&Tambah"));
    mDelButton = new QPushButton(tr("&Delete"));
    mEditButton = new QPushButton(tr("&Ubah"));

    QVBoxLayout *buttonBox = new QVBoxLayout;
    buttonBox->addWidget(okButton);
    buttonBox->addSpacing(10);
    buttonBox->addWidget(mAddButton);
    buttonBox->addWidget(mDelButton);
    buttonBox->addWidget(mEditButton);
    buttonBox->addStretch();

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(mUsersView, 0, 0, 1, 1);
    layout->addLayout(buttonBox, 0, 1, 1, 1);

    setLayout(layout);
    setWindowTitle("Manage Users");
    setWindowIcon(QIcon(":/images/user.png"));
    resize(QSize(700, 450));

    connect(okButton, SIGNAL(clicked()), SLOT(accept()));
    connect(mAddButton, SIGNAL(clicked()), SLOT(addUser()));
    connect(mDelButton, SIGNAL(clicked()), SLOT(deleteUser()));
    connect(mEditButton, SIGNAL(clicked()), SLOT(editUser()));
    connect(mUsersView, SIGNAL(clicked(QModelIndex)), SLOT(updateButton()));

    updateModel();
}

void UserManageDialog::reset()
{
    mSelectedUserId = 0;
    mSelectedUserGroup = 0;

    mDelButton->setEnabled(false);
    mEditButton->setEnabled(false);
}

void UserManageDialog::updateModel()
{
    Engine *engine = Engine::instance();
    CData *data = static_cast<CData *>(engine->data());

    QList<User> userList = data->userList();

    delete mUsersView->model();
    QStandardItemModel *model = new QStandardItemModel(userList.count(), 5, mUsersView);
    model->setHeaderData(1, Qt::Horizontal, "Username");
    model->setHeaderData(2, Qt::Horizontal, "Nama Lengkap");
    model->setHeaderData(4, Qt::Horizontal, "Group");

    int row = 0;
    foreach (const User &user, userList) {
        QStandardItem *idItem = new QStandardItem(QString::number(user.id));
        idItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
        model->setItem(row, 0, idItem);

        QStandardItem *usernameItem = new QStandardItem(user.username);
        usernameItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
        model->setItem(row, 1, usernameItem);

        QStandardItem *fullnameItem = new QStandardItem(user.fullname);
        model->setItem(row, 2, fullnameItem);

        QStandardItem *groupItem = new QStandardItem(QString::number(user.group));
        model->setItem(row, 3, groupItem);

        QString groupName;
        if ( user.group == GroupAdministrator )
            groupName = "Administrators";
        else if ( user.group == GroupUser )
            groupName = "User";

        QStandardItem *groupNameItem = new QStandardItem(groupName);
        groupNameItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
        model->setItem(row, 4, groupNameItem);
        row++;
    }

    mUsersView->setModel(model);

    mUsersView->setColumnHidden(0, true);
    mUsersView->setColumnHidden(3, true);
    mUsersView->setColumnWidth(1, 90);
    mUsersView->setColumnWidth(2, 350);
    mUsersView->setColumnWidth(4, 90);

    reset();
}

void UserManageDialog::updateButton()
{
    int row = mUsersView->currentIndex().row();
    QStandardItemModel *model = static_cast<QStandardItemModel *>(mUsersView->model());

    int tempUserId = model->item(row, 0)->text().toInt();
    int tempUserGroup = model->item(row, 3)->text().toInt();

    mDelButton->setEnabled(true);
    mEditButton->setEnabled(true);

    mSelectedUserId = tempUserId;
    mSelectedUserGroup = tempUserGroup;
}

void UserManageDialog::addUser()
{
    UserUpdateDialog dialog(this);

    bool done = false;
    while ( !done ) {
        if ( dialog.exec() == QDialog::Accepted ) {
            Engine *engine = Engine::instance();
            CData *data = static_cast<CData *>(engine->data());

            if ( dialog.fullname().isEmpty()
                 || dialog.username().isEmpty()
                 || dialog.password().isEmpty()
                 || dialog.rePassword().isEmpty()
                 || dialog.group() == 0 ) {
                QMessageBox::information(this, "Info", "Isian tidak boleh kosong.");
            }
            else if (data->user(dialog.username()).id) {
                QMessageBox::information(this, "Info", "Username sudah ada.");
            }
            else if (dialog.password().length() < 5) {
                QMessageBox::information(this, "Info", "Panjang minimal password adalah 5 karakter.");
            }
            else if (dialog.password() != dialog.rePassword() ) {
                QMessageBox::information(this, "Info", "Password tidak sama.");
            }
            else {
                if (data->addUser(dialog.username(), dialog.password(), dialog.fullname(), dialog.group())) {
                    updateModel();
                    QMessageBox::information(this, "Info", "User baru berhasil ditambahkan.");
                    done = true;
                }
                else {
                    QMessageBox::information(this, "Info", "User baru gagal ditambahkan.");
                }
            }
        }
        else {
            done = true;
        }
    }
}

void UserManageDialog::editUser()
{
    UserUpdateDialog dialog(this);
    dialog.setMode(UserUpdateDialog::EditMode);
    dialog.setUserId(mSelectedUserId);

    bool done = false;
    while ( !done ) {
        if ( dialog.exec() == QDialog::Accepted ) {
            if ( dialog.fullname().isEmpty()
                 || dialog.username().isEmpty()
                 || dialog.password().isEmpty()
                 || dialog.rePassword().isEmpty()
                 || dialog.group() == 0 ) {
                QMessageBox::information(this, "Info", "Isian tidak boleh kosong.");
            }
            else if ( dialog.password().length() < 5 ) {
                QMessageBox::information(this, "Info", "Panjang minimal password adalah 5 karakter.");
            }
            else if ( dialog.password() != dialog.rePassword() ) {
                QMessageBox::information(this, "Info", "Password tidak sama.");
            }
            else {
                Engine *engine = Engine::instance();
                if (static_cast<CData *>(engine->data())->updateUser(mSelectedUserId, dialog.username(), dialog.password(), dialog.fullname(), dialog.group()) ) {
                    updateModel();
                    QMessageBox::information(this, "Info", "User berhasil diubah.");
                    done = true;
                }
                else {
                    QMessageBox::warning(this, "Error", "User gagal diubah.");
                }
            }
        }
        else {
            done = true;
        }
    }
}

void UserManageDialog::deleteUser()
{
    if ( QMessageBox::question(this, "Konfirmasi", "Apakah Anda yakin akan menghapus data user ini?", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes ) {
        Engine *engine = Engine::instance();
        if (static_cast<CData *>(engine->data())->deleteUser(mSelectedUserId) ) {
            updateModel();
            QMessageBox::information(this, "Info", "Data User berhasil dihapus.");
        }
        else {
            QMessageBox::warning(this, "Error", "Data User gagal dihapus.");
        }
    }
}
