#include "mainwindow.h"

#include <QApplication>
#include <QMessageBox>
#include <QDebug>

#include "engine.h"
#include "logindialog.h"

int main(int argc, char *argv[])
{
    QLocale::setDefault(QLocale::Indonesian);
    QApplication a(argc, argv);

    Engine *engine = Engine::instance();
    if (!engine->connectDatabase())
        return 1;

    LoginDialog dialog;
    do {
        static int count(0);
        if (dialog.exec() == QDialog::Rejected)
            return 1;

        if (engine->login(dialog.username(), dialog.password()))
            break;

        QMessageBox::warning(nullptr, "Login", "Username atau Password salah.");
        if (++count == 3)
            return 1;
    } while (true);

    MainWindow w;
    w.start();

    return a.exec();
}
