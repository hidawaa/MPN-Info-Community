#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>

class QStackedWidget;
class QListWidget;
class QSettings;
class QLineEdit;
class QComboBox;
class QListWidgetItem;
class QCheckBox;
class QSpinBox;
class QRadioButton;
class MapsPositionDialog;
class GeneralPage : public QWidget
{
    Q_OBJECT
public:
    explicit GeneralPage(QWidget *parent = nullptr);

public slots:
    void save();
    void load();

private slots:
    void browseDownloadDirectory();
    void browseSharedDataDirectory();
    void updateMaps();
    void loadMap();
    void updateKpp();

private:
    QComboBox *mKanwilComboBox;
    QComboBox *mKppComboBox;
    QLineEdit *mAlamatEdit;
    QLineEdit *mTelpEdit;
    QLineEdit *mKotaEdit;
    QLineEdit *mWpjEdit;
    QLineEdit *mKpEdit;
    QLineEdit *mMapsDefaultLatitudeEdit;
    QLineEdit *mMapsDefaultLongitudeEdit;
    QLineEdit *mMapsDefaultZoomEdit;
    QCheckBox *mDownloadSplitCheckBox;
    QSpinBox *mDownlaodTimeOutSpinBox;
    QLineEdit *mDownloadDirEdit;
    QLineEdit *mSharedDataDirEdit;
    QCheckBox *mSharedTemplateCheckBox;

    MapsPositionDialog *mMapsDialog;
};

class HitungPage : public QWidget
{
    Q_OBJECT
public:
    explicit HitungPage(QWidget *parent = nullptr);

public slots:
    void save();
    void load();

private slots:
    void updateInput();

private:
    QRadioButton *mOldRadioButton;
    QRadioButton *mNewRadioButton;
    QLineEdit *mGrowthIndexLineEdit;
    QLineEdit *mCapaianIndexLineEdit;

    QRadioButton *mNpwpTetapRadioButton;
    QRadioButton *mKodeKppPbkRadioButton;
};

class SettingsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SettingsDialog(QWidget *parent = nullptr);

private slots:
    void save();
    void load();
    void changePage(QListWidgetItem *current, QListWidgetItem *previous);

private:
    QStackedWidget *mStackedWidget;
    QListWidget *mListWidget;

    GeneralPage *mGeneralPage;
    HitungPage *mHitungPage;
};

#endif // SETTINGSDIALOG_H
