#include "changelogdialog.h"

#include <QTextEdit>
#include <QVBoxLayout>
#include <QIcon>
#include <QFile>

ChangelogDialog::ChangelogDialog(QWidget *parent) :
    QDialog(parent)
{
    mChangelogEdit = new QTextEdit;
    mChangelogEdit->setReadOnly(true);

    QFile changelogFile(":/changelog.txt");
    if (changelogFile.open(QIODevice::ReadOnly | QIODevice::Text))
        mChangelogEdit->setText(changelogFile.readAll());

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(mChangelogEdit);

    setLayout(layout);
    setWindowTitle("Changelog");
    setWindowIcon(QIcon(":/images/log.png"));
    resize(400, 500);
}
