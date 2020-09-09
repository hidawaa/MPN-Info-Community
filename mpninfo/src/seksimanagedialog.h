#ifndef SEKSIMANAGEDIALOG_H
#define SEKSIMANAGEDIALOG_H

#include <QDialog>

class QTableView;
class QComboBox;
class SeksiManageDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SeksiManageDialog(QWidget *parent = 0);

    void reset();

private slots:
    void addSeksi();
    void editSeksi();
    void deleteSeksi();

private slots:
    void updateModel();
    void updateButton();

private:
    QTableView *mResultView;

    QComboBox *mKantorComboBox;

    QPushButton *mAddButton;
    QPushButton *mDelButton;
    QPushButton *mEditButton;

    int mSelectedSeksiId;
};

#endif // SEKSIMANAGEDIALOG_H
