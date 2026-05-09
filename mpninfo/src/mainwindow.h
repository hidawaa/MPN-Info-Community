#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>
#include <QAction>
#include <QMenu>

#include <window.h>
#include <interface.h>

class MainWindow : public QMainWindow, public Window
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void addPage(PagePtr page, const QString &title);

public slots:
    void start();
    void logout();
    void quit();

    void onMap();
    void onKlu();

    void onChangePassword();
    void onManageSeksi();
    void onManagePegawai();
    void onManageUser();
    void onSettings();

    void onChangelog();
    void onManual();
    void onAbout();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void removePage(int index);
    void onAddOnActionTriggered();

private:
    void processAddOn(AddOnPtr addOn);

private:
    QTabWidget *mTabWidget;
    QMap<QString, QMenu *> mGroupMenuMap;
    QMap<QAction *, QString> mActionMap;
    QHash<QWidget *, PagePtr> mPageHash;
};
#endif // MAINWINDOW_H
