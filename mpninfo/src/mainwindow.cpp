#include "mainwindow.h"

#include <QCloseEvent>
#include <QGridLayout>
#include <QTabWidget>
#include <QMenuBar>
#include <QTimer>
#include <QMessageBox>
#include <QDesktopServices>
#include <QUrl>
#include <QDebug>

#include <interface.h>

#include "engine.h"
#include "klusearchdialog.h"
#include "mapsearchdialog.h"
#include "passwordchangedialog.h"
#include "seksimanagedialog.h"
#include "pegawaimanagedialog.h"
#include "usermanagedialog.h"
#include "settingsdialog.h"
#include "changelogdialog.h"
#include "aboutdialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    mTabWidget = new QTabWidget;
    mTabWidget->setTabsClosable(true);

    setWindowIcon(QIcon(":/images/logo.png"));
    setCentralWidget(mTabWidget);
    menuBar()->show();
    resize(800, 600);

    connect(mTabWidget, &QTabWidget::tabCloseRequested, this, &MainWindow::removePage);
}

MainWindow::~MainWindow()
{
    while (mTabWidget->currentIndex() != -1)
        removePage(mTabWidget->currentIndex());
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    event->ignore();
    quit();
}

void MainWindow::addPage(PagePtr page, const QString &title)
{
    QWidget *widget = page.data();
    mTabWidget->addTab(widget, title);
    mTabWidget->setCurrentWidget(widget);

    mPageHash[widget] = page;
}

void MainWindow::start()
{
    QMenu *fileMenu = new QMenu("File");

    QAction *logoutAction = new QAction(QIcon(":/images/logoff.png"), "Logout", this);
    QAction *quitAction = new QAction(QIcon(":/images/exit.png"), "Exit", this);

    connect(logoutAction, &QAction::triggered, this, &MainWindow::logout);
    connect(quitAction, &QAction::triggered, this, &MainWindow::quit);

    fileMenu->addAction(logoutAction);
    fileMenu->addAction(quitAction);

    menuBar()->addMenu(fileMenu);

    Engine *engine = Engine::instance();

    QMap<QString, QAction *> sortMap;
    QList<AddOnPtr> addOnExecList;
    foreach (const QString &name, engine->availableAddOns()) {
        AddOnPtr addOnPtr = engine->addOn(name);

        if (addOnPtr->permission() != 0 && !(addOnPtr->permission() & engine->user().permission))
            continue;

        if (addOnPtr->loadFlags() & AddOnCreateMenu) {
            QString group = addOnPtr->group();
            QString title = addOnPtr->title();

            QMenu *groupMenu(nullptr);
            if (mGroupMenuMap.contains(group))
                groupMenu = mGroupMenuMap[group];
            else {
                groupMenu = new QMenu(group);
                mGroupMenuMap[group] = groupMenu;
            }

            QIcon icon(QString(":/images/%1.png").arg(addOnPtr->name()));
            QAction *action = new QAction(title, this);
            if (!icon.isNull())
                action->setIcon(icon);
                
            mActionMap[action] = addOnPtr->name();
            sortMap[title] = action;
        }

        if (addOnPtr->loadFlags() & AddOnExecAfterLogin)
            addOnExecList << addOnPtr;
    }

    if (mGroupMenuMap.contains("Home")) {
        menuBar()->addMenu(mGroupMenuMap["Home"]);
    }

    QMapIterator<QString, QMenu *> groupMenuIterator(mGroupMenuMap);
    while (groupMenuIterator.hasNext()) {
        groupMenuIterator.next();
        
        if (groupMenuIterator.key() == "Home")
            continue;

        menuBar()->addMenu(groupMenuIterator.value());
    }

    {
        QMenu *referensiMenu = new QMenu("Referensi");
        QMenu *settingsMenu = new QMenu("Pengaturan");
        QMenu *helpMenu = new QMenu("Bantuan");

        QAction *kluAction = new QAction(QIcon(":/images/log.png"), "KLU", this);
        QAction *mapAction = new QAction(QIcon(":/images/book.png"), "Kode MAP", this);

        connect(kluAction, &QAction::triggered, this, &MainWindow::onKlu);
        connect(mapAction, &QAction::triggered, this, &MainWindow::onMap);

        QAction *changelogAction = new QAction(QIcon(":/images/log.png"), "Changelog", this);
        QAction *manualAction = new QAction(QIcon(":/images/book.png"), "Manual", this);
        QAction *about = new QAction(QIcon(":/images/info.png"), "About", this);

        connect(changelogAction, &QAction::triggered, this, &MainWindow::onChangelog);
        connect(manualAction, &QAction::triggered, this, &MainWindow::onManual);
        connect(about, &QAction::triggered, this, &MainWindow::onAbout);

        referensiMenu->addAction(kluAction);
        referensiMenu->addAction(mapAction);

        helpMenu->addAction(changelogAction);
        helpMenu->addAction(manualAction);
        helpMenu->addAction(about);

        QAction *changePassword = new QAction(QIcon(":/images/auth.png"), "Ganti Password", this);
        settingsMenu->addAction(changePassword);
        settingsMenu->addSeparator();

        connect(changePassword, &QAction::triggered, this, &MainWindow::onChangePassword);

        if (engine->user().group == GroupAdministrator) {
            QAction *seksiAction = new QAction(QIcon(":/images/pegawai.png"), "Manage Seksi", this);
            QAction *pegawaiAction = new QAction(QIcon(":/images/pegawai.png"), "Manage Pegawai", this);
            QAction *userAction = new QAction(QIcon(":/images/user.png"), "Manage User", this);
            QAction *settingsAction = new QAction(QIcon(":/images/settings.png"), "Settings", this);

            connect(seksiAction, &QAction::triggered, this, &MainWindow::onManageSeksi);
            connect(pegawaiAction, &QAction::triggered, this, &MainWindow::onManagePegawai);
            connect(userAction, &QAction::triggered, this, &MainWindow::onManageUser);
            connect(settingsAction, &QAction::triggered, this, &MainWindow::onSettings);

            settingsMenu->addAction(seksiAction);
            settingsMenu->addAction(pegawaiAction);
            settingsMenu->addAction(userAction);
            settingsMenu->addSeparator();
            settingsMenu->addAction(settingsAction);
        }

        menuBar()->addMenu(referensiMenu);
        menuBar()->addMenu(settingsMenu);
        menuBar()->addMenu(helpMenu);
    }

    QMapIterator<QString, QAction *> titleIterator(sortMap);
    while (titleIterator.hasNext()) {
        titleIterator.next();

        QAction *action = titleIterator.value();
        AddOnPtr addOn = engine->addOn(mActionMap[action]);
        mGroupMenuMap[addOn->group()]->addAction(action);

        connect(action, &QAction::triggered, this, &MainWindow::onAddOnActionTriggered);
    }

    foreach (AddOnPtr addOn, addOnExecList) {
        if (addOn->type() == AddOnPage || addOn->type() == AddOnWidget) {
            processAddOn(addOn);
        }
    }

    showMaximized();

    QTimer::singleShot(0, [engine]() {
        engine->processAddOnsAfterLogin();
    });
}

void MainWindow::removePage(int index)
{
    QWidget *widget = mTabWidget->widget(index);

    mTabWidget->removeTab(index);
    mPageHash.remove(widget);
}

void MainWindow::processAddOn(AddOnPtr addOn)
{
    if (addOn->type() == AddOnPage)
        addPage(addOn->newPage(), addOn->title());
    else if (addOn->type() == AddOnProcess)
        addOn->newProcess()->run();
}

void MainWindow::onAddOnActionTriggered()
{
    QAction *action = static_cast<QAction *>(QObject::sender());

    Engine *engine = Engine::instance();
    AddOnPtr addOn = engine->addOn(mActionMap[action]);
    processAddOn(addOn);
}

void MainWindow::logout()
{
    Engine *engine = Engine::instance();
    engine->logout();
}

void MainWindow::quit()
{
    Engine *engine = Engine::instance();
    engine->quit();
}

void MainWindow::onMap()
{
    MapSearchDialog dialog(this);
    dialog.exec();
}

void MainWindow::onKlu()
{
    KluSearchDialog dialog(this);
    dialog.exec();
}

void MainWindow::onChangePassword()
{
    PasswordChangeDialog dialog(this);
    dialog.exec();
}

void MainWindow::onManageSeksi()
{
    SeksiManageDialog dialog(this);
    dialog.exec();
}

void MainWindow::onManagePegawai()
{
    PegawaiManageDialog dialog(this);
    dialog.exec();
}

void MainWindow::onManageUser()
{
    UserManageDialog dialog(this);
    dialog.exec();
}

void MainWindow::onSettings()
{
    SettingsDialog dialog(this);
    dialog.exec();
}

void MainWindow::onChangelog()
{
    ChangelogDialog dialog(this);
    dialog.exec();
}

void MainWindow::onManual()
{
    QDesktopServices::openUrl(QUrl("https://docs.google.com/document/d/1C8pFuaumiVAtmLBeoKNSPvqTpZgLU1uZ3-uHG8XhX_k/edit"));
}

void MainWindow::onAbout()
{
    AboutDialog dialog(this);
    dialog.exec();
}

