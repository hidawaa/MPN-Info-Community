#include "mainwindow.h"

#include <QApplication>
#include <QMessageBox>
#include <QDebug>
#include <QTimer>

#include "engine.h"
#include "logindialog.h"

int main(int argc, char *argv[])
{
    QLocale::setDefault(QLocale::Indonesian);
    QApplication a(argc, argv);

    Engine *engine = Engine::instance();
    engine->loadAddons();
    engine->processAddOnsAtStart();

    if (!engine->connectDatabase())
        return 1;

    engine->processAddOnsBeforeLogin();

    a.setQuitOnLastWindowClosed(false);

    MainWindow *mainWindow = nullptr;

    auto showLoginFlow = [&mainWindow, &a, engine]() {
        if (mainWindow) {
            mainWindow->deleteLater();
            mainWindow = nullptr;
        }

        LoginDialog dialog;

        QString lastUsername = engine->settings()->value(IDS_GENERAL_LAST_USER).toString();
        if (!lastUsername.isEmpty())
            dialog.setUsername(lastUsername);

        int count = 0;
        bool loggedIn = false;
        do {
            if (dialog.exec() == QDialog::Rejected) {
                a.exit(1);
                return;
            }

            if (engine->login(dialog.username(), dialog.password())) {
                loggedIn = true;
                break;
            }

            QMessageBox::warning(nullptr, "Login", "Username atau Password salah.");
            if (++count == 3) {
                a.exit(1);
                return;
            }
        } while (true);

        if (loggedIn) {
            engine->settings()->setValue(IDS_GENERAL_LAST_USER, dialog.username());

            if(!engine->load()) {
                a.exit(1);
                return;
            }

            mainWindow = new MainWindow;
            engine->setWindow(mainWindow);
            mainWindow->start();
        }
    };

    QObject::connect(engine, &Engine::loggedOut, &a, showLoginFlow);
    QTimer::singleShot(0, &a, showLoginFlow);

    return a.exec();
}
