#ifndef CHANGELOGDIALOG_H
#define CHANGELOGDIALOG_H

#include <QDialog>

class QTextEdit;
class ChangelogDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ChangelogDialog(QWidget *parent = 0);

private:
    QTextEdit *mChangelogEdit;
};

#endif // CHANGELOGDIALOG_H
