#include "aboutdialog.h"

#include <QLabel>
#include <QGridLayout>
#include <QTabWidget>
#include <QTextEdit>
#include <QLineEdit>
#include <QFile>

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent, Qt::WindowTitleHint | Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint)
{
    QTabWidget *tab = new QTabWidget;

    QWidget *aboutWidget = new QWidget;
    {
        QLabel *logoLabel = new QLabel;
        logoLabel->setPixmap(QPixmap(QString(":/images/logo-big.png")));

        QLabel *appNameLabel = new QLabel(QString("\n%1 v%2 (%3)").arg("MPN-Info").arg("0.20.0").arg(QSysInfo::buildCpuArchitecture()));

        QFont titleFont;
        titleFont.setPointSize(11);
        titleFont.setWeight(QFont::Bold);
        appNameLabel->setFont(titleFont);

        QLabel *buildLabel = new QLabel(QString("Built on %1 at %2\n").arg(__DATE__, __TIME__));
        QLabel *authorLabel = new QLabel(QString("Dibuat Oleh:\n%1\n%2\n").arg("Ichdyan Thalasa", "ichdyan.thalasa@gmail.com"));
        authorLabel->setAlignment(Qt::AlignCenter);
        QLabel *testerLabel = new QLabel(QString("Tester, Marketing, dan Manual:\n%1").arg("Yogi Iskandar"));
        testerLabel->setAlignment(Qt::AlignCenter);

        QGridLayout *aboutLayout = new QGridLayout;
        aboutLayout->addWidget(logoLabel, 0, 0, 1, 1, Qt::AlignCenter);
        aboutLayout->addWidget(appNameLabel, 1, 0, 1, 1, Qt::AlignCenter);
        aboutLayout->addWidget(buildLabel, 2, 0, 1, 1, Qt::AlignCenter);
        aboutLayout->addWidget(authorLabel, 3, 0, 1, 1, Qt::AlignCenter);
        aboutLayout->addWidget(testerLabel, 4, 0, 1, 1, Qt::AlignCenter);

        aboutWidget->setLayout(aboutLayout);
    }

    tab->addTab(aboutWidget, "Aplikasi");

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(tab);

    setLayout(layout);
    setWindowTitle(QString("About"));
    setWindowIcon(QIcon(":/images/info.png"));
    resize(300, 250);
}
