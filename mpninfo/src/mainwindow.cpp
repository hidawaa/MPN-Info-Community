#include "mainwindow.h"

#include <QGridLayout>
#include <QTabWidget>
#include <QMenuBar>
#include <QTimer>
#include <QDebug>

#include <interface.h>
#include "engine.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    mTabWidget = new QTabWidget;
    mTabWidget->setTabsClosable(true);

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

void MainWindow::addPage(Page *page, const QString &title)
{
    mTabWidget->addTab(page, title);
}

void MainWindow::start()
{
    showMaximized();
    QTimer::singleShot(0, this, &MainWindow::load);
}

void MainWindow::load()
{
    Engine *engine = Engine::instance();
    engine->loadData();
    engine->loadPlugins();

    QMap<QString, QAction *> sortMap;
    QList<AddOnPtr> addOnExecList;
    foreach (const QString &name, engine->availableAddOns()) {
        AddOnPtr addOnPtr = engine->addOn(name);

        if (addOnPtr->permission() != 0 && !(addOnPtr->permission() & engine->user().permission))
            continue;

        if (addOnPtr->loadFlags() & AddOnLoadCreateMenu) {
            QString group = addOnPtr->group();
            QString title = addOnPtr->title();

            QMenu *groupMenu(nullptr);
            if (mGroupMenuMap.contains(group))
                groupMenu = mGroupMenuMap[group];
            else {
                groupMenu = new QMenu(group);
                mGroupMenuMap[group] = groupMenu;
            }

            QAction *action = new QAction(title, this);
            mActionMap[action] = addOnPtr->name();
            sortMap[title] = action;
        }

        if (addOnPtr->loadFlags() & AddOnLoadExecAfterLogin)
            addOnExecList << addOnPtr;
    }

    QMapIterator<QString, QMenu *> groupMenuIterator(mGroupMenuMap);
    while (groupMenuIterator.hasNext()) {
        groupMenuIterator.next();

        menuBar()->addMenu(groupMenuIterator.value());
    }

    QMapIterator<QString, QAction *> titleIterator(sortMap);
    while (titleIterator.hasNext()) {
        titleIterator.next();

        QAction *action = titleIterator.value();
        AddOnPtr addOn = engine->addOn(mActionMap[action]);
        mGroupMenuMap[addOn->group()]->addAction(action);

        connect(action, &QAction::triggered, this, &MainWindow::onAddOnActionTriggered);
    }

    QTimer::singleShot(0, [this, addOnExecList]() {
        foreach (AddOnPtr addOn, addOnExecList)
            processAddOn(addOn);
    });
}

void MainWindow::removePage(int index)
{
    QWidget *page = mTabWidget->widget(index);

    mTabWidget->removeTab(index);
    delete page;
}

void MainWindow::processAddOn(AddOnPtr addOn)
{
    if (addOn->type() == AddOnPage)
        mTabWidget->addTab(addOn->newPage(), addOn->title());
    else if (addOn->type() == AddOnProcess)
        QScopedPointer<Process>(addOn->newProcess())->run();
}

void MainWindow::onAddOnActionTriggered()
{
    QAction *action = static_cast<QAction *>(QObject::sender());

    Engine *engine = Engine::instance();
    AddOnPtr addOn = engine->addOn(mActionMap[action]);
    processAddOn(addOn);
}
