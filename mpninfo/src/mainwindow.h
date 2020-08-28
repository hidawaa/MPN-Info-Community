#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>

#include <window.h>
#include <interface.h>

class MainWindow : public QMainWindow, public Window
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void start();

    void addPage(PagePtr page, const QString &title);

private slots:
    void load();
    void removePage(int index);
    void onAddOnActionTriggered();

private:
    void processAddOn(AddOnPtr addOn);

private:
    QTabWidget *mTabWidget;
    QMap<QString, QMenu *> mGroupMenuMap;
    QMap<QAction *, QString> mActionMap;
};
#endif // MAINWINDOW_H
