#include "settingsdialog.h"

#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QListWidget>
#include <QStackedWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QCheckBox>
#include <QSpinBox>
#include <QFileDialog>
#include <QRadioButton>
#include <QButtonGroup>
#include <QTimer>

#include "engine.h"

GeneralPage::GeneralPage(QWidget *parent) :
    QWidget(parent, Qt::WindowTitleHint | Qt::WindowCloseButtonHint)
{
    Engine *engine = Engine::instance();

    mKanwilComboBox = new QComboBox;
    mKanwilComboBox->addItem("-");
    foreach (const Kantor &kantor, engine->data()->kanwilList())
        mKanwilComboBox->addItem(kantor.kode + " - " + kantor.nama, kantor.kode);
    mKanwilComboBox->setCurrentIndex(0);

    mKppComboBox = new QComboBox;
    mKppComboBox->addItem("-");
    mKppComboBox->setCurrentIndex(0);

    mAlamatEdit = new QLineEdit;
    mTelpEdit = new QLineEdit;
    mKotaEdit = new QLineEdit;
    mWpjEdit = new QLineEdit;
    mKpEdit = new QLineEdit;

    mWpjEdit->setAlignment(Qt::AlignCenter);
    mWpjEdit->setMaximumWidth(30);
    mWpjEdit->setInputMask("00;_");

    mKpEdit->setAlignment(Qt::AlignCenter);
    mKpEdit->setMaximumWidth(30);
    mKpEdit->setInputMask("00;_");

    QGridLayout *kantorLayout = new QGridLayout;
    kantorLayout->addWidget(new QLabel("Kanwil"), 0, 0);
    kantorLayout->addWidget(mKanwilComboBox, 0, 1);
    kantorLayout->addWidget(new QLabel("KPP"), 1, 0);
    kantorLayout->addWidget(mKppComboBox, 1, 1);
    kantorLayout->addWidget(new QLabel("Alamat"), 2, 0);
    kantorLayout->addWidget(mAlamatEdit, 2, 1);
    kantorLayout->addWidget(new QLabel("Telepon"), 3, 0);
    kantorLayout->addWidget(mTelpEdit, 3, 1);
    kantorLayout->addWidget(new QLabel("Kota"), 4, 0);
    kantorLayout->addWidget(mKotaEdit, 4, 1);
    kantorLayout->addWidget(new QLabel("WPJ"), 5, 0);
    kantorLayout->addWidget(mWpjEdit, 5, 1);
    kantorLayout->addWidget(new QLabel("KP"), 6, 0);
    kantorLayout->addWidget(mKpEdit, 6, 1);

    QGroupBox *kantorGroup = new QGroupBox("Detail Kantor");
    kantorGroup->setLayout(kantorLayout);

    mMapsDefaultLatitudeEdit = new QLineEdit;
    mMapsDefaultLongitudeEdit = new QLineEdit;
    mMapsDefaultZoomEdit = new QLineEdit;

    mMapsDefaultZoomEdit->setMaximumWidth(80);
    mMapsDefaultZoomEdit->setAlignment(Qt::AlignCenter);

    mMapsDefaultLatitudeEdit->setEnabled(false);
    mMapsDefaultLongitudeEdit->setEnabled(false);
    mMapsDefaultZoomEdit->setEnabled(false);

    QPushButton *mapsSetButton = new QPushButton("Ubah");

    QHBoxLayout *mapsLayout = new QHBoxLayout;
    mapsLayout->addWidget(new QLabel("Lat"));
    mapsLayout->addWidget(mMapsDefaultLatitudeEdit);
    mapsLayout->addWidget(new QLabel("Lng"));
    mapsLayout->addWidget(mMapsDefaultLongitudeEdit);
    mapsLayout->addWidget(new QLabel("Zoom"));
    mapsLayout->addWidget(mMapsDefaultZoomEdit);
    mapsLayout->addWidget(mapsSetButton);

    QGroupBox *mapsGroupBox = new QGroupBox("Maps Default Position");
    mapsGroupBox->setLayout(mapsLayout);

    mDownloadSplitCheckBox = new QCheckBox;
    mDownloadSplitCheckBox->setText("Split Download File (untuk KPP dengan jumlah data sangat besar.");

    mDownloadDirEdit = new QLineEdit;
    mDownloadDirEdit->setEnabled(false);

    QPushButton *browseDownloadButton = new QPushButton(QIcon(":/images/save-folder.png"), "Browse");
    browseDownloadButton->setIconSize(QSize(16,16));

    mDownlaodTimeOutSpinBox = new QSpinBox;
    mDownlaodTimeOutSpinBox->setRange(5, 1200);
    mDownlaodTimeOutSpinBox->setValue(20);

    QHBoxLayout *timeoutLayout = new QHBoxLayout;
    timeoutLayout->addWidget(new QLabel("Download Timeout"));
    timeoutLayout->addWidget(mDownlaodTimeOutSpinBox);
    timeoutLayout->addWidget(new QLabel("detik"));
    timeoutLayout->addStretch();

    QHBoxLayout *downloadDirLayout = new QHBoxLayout;
    downloadDirLayout->addWidget(new QLabel("Download Directory"));
    downloadDirLayout->addWidget(mDownloadDirEdit);
    downloadDirLayout->addWidget(browseDownloadButton);

    QGridLayout *downloadLayout = new QGridLayout;
    downloadLayout->addWidget(mDownloadSplitCheckBox, 0, 0);
    downloadLayout->addLayout(timeoutLayout, 1, 0);
    downloadLayout->addLayout(downloadDirLayout, 2, 0);

    QGroupBox *downloadBox = new QGroupBox("Download Detail");
    downloadBox->setLayout(downloadLayout);

    mSharedDataDirEdit = new QLineEdit;
    mSharedDataDirEdit->setEnabled(false);

    mSharedTemplateCheckBox = new QCheckBox("Gunakan untuk template dokumen");

    QPushButton *browseSharedDataButton = new QPushButton(QIcon(":/images/save-folder.png"), "Browse");
    browseSharedDataButton->setIconSize(QSize(16,16));

    QHBoxLayout *sharedDataDirLayout = new QHBoxLayout;
    sharedDataDirLayout->addWidget(new QLabel("Shared Data Directory"));
    sharedDataDirLayout->addWidget(mSharedDataDirEdit);
    sharedDataDirLayout->addWidget(browseSharedDataButton);

    QVBoxLayout *sharedLayout = new QVBoxLayout;
    sharedLayout->addLayout(sharedDataDirLayout);
    sharedLayout->addWidget(mSharedTemplateCheckBox);

    QGroupBox *sharedDataBox = new QGroupBox("Shared Data Detail");
    sharedDataBox->setLayout(sharedLayout);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(kantorGroup);
    layout->addWidget(downloadBox);
    layout->addWidget(mapsGroupBox);
    layout->addWidget(sharedDataBox);
    layout->addStretch();

    setLayout(layout);

    connect(mKanwilComboBox, SIGNAL(currentIndexChanged(int)), SLOT(updateKpp()));
    connect(browseDownloadButton, SIGNAL(clicked()), SLOT(browseDownloadDirectory()));
    connect(browseSharedDataButton, SIGNAL(clicked()), SLOT(browseSharedDataDirectory()));
    connect(mapsSetButton, SIGNAL(clicked()), SLOT(updateMaps()));
}

void GeneralPage::browseDownloadDirectory()
{
    QString downloadDir = QFileDialog::getExistingDirectory(this, "Save Directory");
    if (downloadDir.isEmpty())
        return;

    mDownloadDirEdit->setText(QDir::toNativeSeparators(downloadDir));
}

void GeneralPage::browseSharedDataDirectory()
{
    bool done(false);
    QString sharedDir = QDir::fromNativeSeparators(mSharedDataDirEdit->text());

    do
    {
        sharedDir = QFileDialog::getExistingDirectory(this, "Shared Data Directory", sharedDir);
        if (sharedDir.isEmpty())
            done = true;
        else
        {
            QFile file(sharedDir + "/.test");
            if (!file.open(QIODevice::WriteOnly))
                QMessageBox::warning(this, "Shared Data", "Directory yang Anda pilih tidak memiliki hak tulis.");
            else
            {
                file.close();
                file.remove();
                done = true;

                mSharedDataDirEdit->setText(QDir::toNativeSeparators(sharedDir));
            }
        }
    } while (!done);
}

void GeneralPage::updateMaps()
{
    /*
    MapsPositionDialog dialog(this);
    dialog.showMaximized();
    mMapsDialog = &dialog;
    QTimer::singleShot(500, this, SLOT(loadMap()));
    if (dialog.exec() == QDialog::Rejected)
        return;

    MapsLatLng newPos = dialog.position();
    mMapsDefaultLatitudeEdit->setText(QString("%1").arg(newPos.lat(), 0, 'f'));
    mMapsDefaultLongitudeEdit->setText(QString("%1").arg(newPos.lng(), 0, 'f'));
    mMapsDefaultZoomEdit->setText(QString::number(dialog.zoom()));
    mMapsDialog = 0;
    */
}

void GeneralPage::loadMap()
{
    /*
    if (!mMapsDialog)
        return;

    double lat = mMapsDefaultLatitudeEdit->text().toDouble();
    double lng = mMapsDefaultLongitudeEdit->text().toDouble();
    int zoom = mMapsDefaultZoomEdit->text().toDouble();

    mMapsDialog->load(MapsLatLng(lat, lng), zoom);
    */
}

void GeneralPage::updateKpp()
{
    Engine *engine = Engine::instance();

    mKppComboBox->clear();
    mKppComboBox->addItem("-");

    QString kanwil = mKanwilComboBox->currentData().toString();
    foreach (const Kantor &kantor,  engine->data()->kppList(kanwil)) {
        if (kantor.kode == kanwil)
            continue;

        mKppComboBox->addItem(kantor.kode + " - " + kantor.nama, kantor.kode);
    }
}

void GeneralPage::save()
{
    Engine *engine = Engine::instance();
    do {
        QString kanwil = mKanwilComboBox->currentData().toString();
        QString kpp = mKppComboBox->currentData().toString();

        if (kanwil.isEmpty())
            break;

        QString kodeKantor = kpp.isEmpty()? kanwil : kpp;
        engine->databaseSettings()->setValue(IDS_SERVER_KANTOR_KODE, kodeKantor);
        engine->setKantor(kodeKantor);
    } while(false);

    engine->databaseSettings()->setValue(IDS_SERVER_KANTOR_ALAMAT, mAlamatEdit->text());
    engine->databaseSettings()->setValue(IDS_SERVER_KANTOR_TELEPON, mTelpEdit->text());
    engine->databaseSettings()->setValue(IDS_SERVER_KANTOR_KOTA, mKotaEdit->text());
    engine->databaseSettings()->setValue(IDS_SERVER_KANTOR_WPJ, mWpjEdit->text());
    engine->databaseSettings()->setValue(IDS_SERVER_KANTOR_KP, mKpEdit->text());

    engine->databaseSettings()->setValue(IDS_SERVER_DOWNLAOD_SPLIT, mDownloadSplitCheckBox->isChecked());
    engine->settings()->setValue(IDS_GENERAL_DOWNLOAD_TIMEOUT, mDownlaodTimeOutSpinBox->value());
    engine->settings()->setValue(IDS_GENERAL_DOWNLOAD_DIR, QDir::fromNativeSeparators(mDownloadDirEdit->text()));

    {
        QString lat = mMapsDefaultLatitudeEdit->text();
        QString lng = mMapsDefaultLongitudeEdit->text();

        engine->databaseSettings()->setValue(IDS_SERVER_MAPS_DEFAULT_POSITION, lat + "," + lng);
        engine->databaseSettings()->setValue(IDS_SERVER_MAPS_DEFAULT_ZOOM, mMapsDefaultZoomEdit->text());
    }

    engine->databaseSettings()->setValue(IDS_SERVER_SHARED_DATA_FOLDER_PATH, QDir::fromNativeSeparators(mSharedDataDirEdit->text()));
    engine->databaseSettings()->setValue(IDS_SERVER_SHARED_DATA_AS_TEMPLATE, mSharedTemplateCheckBox->isChecked());
}

void GeneralPage::load()
{
    Engine *engine = Engine::instance();

    QString kodeKantor = engine->databaseSettings()->value(IDS_SERVER_KANTOR_KODE).toString();
    if (engine->data()->kantorMap().contains(kodeKantor)) {
        Kantor kantor =  engine->data()->kantor(kodeKantor);
        mKanwilComboBox->setCurrentIndex(mKanwilComboBox->findData(kantor.kanwil));
        mKppComboBox->setCurrentIndex(mKppComboBox->findData(kantor.kode));
    }

    mAlamatEdit->setText(engine->databaseSettings()->value(IDS_SERVER_KANTOR_ALAMAT).toString());
    mTelpEdit->setText(engine->databaseSettings()->value(IDS_SERVER_KANTOR_TELEPON).toString());
    mKotaEdit->setText(engine->databaseSettings()->value(IDS_SERVER_KANTOR_KOTA).toString());
    mWpjEdit->setText(engine->databaseSettings()->value(IDS_SERVER_KANTOR_WPJ).toString());
    mKpEdit->setText(engine->databaseSettings()->value(IDS_SERVER_KANTOR_KP).toString());

    mDownloadDirEdit->setText(QDir::toNativeSeparators(engine->settings()->value(IDS_GENERAL_DOWNLOAD_DIR).toString()));

    mDownloadSplitCheckBox->setChecked(engine->databaseSettings()->value(IDS_SERVER_DOWNLAOD_SPLIT).toBool());
    int timeout(20);
    if (engine->settings()->value(IDS_GENERAL_DOWNLOAD_TIMEOUT).isValid())
        timeout = engine->settings()->value(IDS_GENERAL_DOWNLOAD_TIMEOUT).toInt();
    mDownlaodTimeOutSpinBox->setValue(timeout);

    {
        QString posString = engine->databaseSettings()->value(IDS_SERVER_MAPS_DEFAULT_POSITION).toString();
        QString zoomString = engine->databaseSettings()->value(IDS_SERVER_MAPS_DEFAULT_ZOOM).toString();

        double lat(-0.878872), lng(117.729492);
        int zoom(5);

        if (!posString.isEmpty())
        {
            QStringList posList = posString.split(",");
            lat = posList[0].toDouble();
            lng = posList.count() > 1? posList[1].toDouble() : lng;
        }

        if (!zoomString.isEmpty())
            zoom = zoomString.toInt();

        mMapsDefaultLatitudeEdit->setText(QString("%1").arg(lat, 0, 'f'));
        mMapsDefaultLongitudeEdit->setText(QString("%1").arg(lng, 0, 'f'));
        mMapsDefaultZoomEdit->setText(QString::number(zoom));
    }

    mSharedDataDirEdit->setText(QDir::toNativeSeparators(engine->databaseSettings()->value(IDS_SERVER_SHARED_DATA_FOLDER_PATH).toString()));
    mSharedTemplateCheckBox->setChecked(engine->databaseSettings()->value(IDS_SERVER_SHARED_DATA_AS_TEMPLATE).toBool());
}

HitungPage::HitungPage(QWidget *parent) :
    QWidget(parent, Qt::WindowTitleHint | Qt::WindowCloseButtonHint)
{
    mOldRadioButton = new QRadioButton;
    mNewRadioButton = new QRadioButton;
    mOldRadioButton->setChecked(true);

    QButtonGroup *indeksButtonGroup = new QButtonGroup(this);
    indeksButtonGroup->addButton(mOldRadioButton);
    indeksButtonGroup->addButton(mNewRadioButton);

    mGrowthIndexLineEdit = new QLineEdit;
    mGrowthIndexLineEdit->setMaximumWidth(50);
    mGrowthIndexLineEdit->setInputMask("0,00;_");
    mGrowthIndexLineEdit->setAlignment(Qt::AlignCenter);

    mCapaianIndexLineEdit = new QLineEdit;
    mCapaianIndexLineEdit->setMaximumWidth(50);
    mCapaianIndexLineEdit->setInputMask("0,00;_");
    mCapaianIndexLineEdit->setAlignment(Qt::AlignCenter);

    QHBoxLayout *indeksLayout = new QHBoxLayout;
    indeksLayout->addWidget(mGrowthIndexLineEdit);
    indeksLayout->addWidget(new QLabel(" x Pertumbuhan + "));
    indeksLayout->addWidget(mCapaianIndexLineEdit);
    indeksLayout->addWidget(new QLabel(" x Pencapaian"));
    indeksLayout->addStretch();
    indeksLayout->setSpacing(1);

    QGridLayout *hitungLayout = new QGridLayout;
    hitungLayout->addWidget(mOldRadioButton, 0, 0);
    hitungLayout->addWidget(new QLabel("Rumus Lama"), 0, 1);
    hitungLayout->addLayout(indeksLayout, 0, 2);
    hitungLayout->addWidget(mNewRadioButton, 1, 0);
    hitungLayout->addWidget(new QLabel("Rumus Baru"), 1, 1);

    QLabel *rumusBaruLabel = new QLabel("0,10 x Peranan Penerimaan + 0,90 x ( 0,60 x Pertumbuhan +  0,40 x Pencapaian)");

    QFont rumusFont = rumusBaruLabel->font();
    rumusFont.setPixelSize(9);
    rumusBaruLabel->setFont(rumusFont);

    hitungLayout->addWidget(rumusBaruLabel, 2, 1, 1, 2, Qt::AlignCenter);

    QGroupBox *hitungBox = new QGroupBox("Detail Perhitungan");
    hitungBox->setLayout(hitungLayout);

    mNpwpTetapRadioButton = new QRadioButton("NPWP Tetap");
    mKodeKppPbkRadioButton = new QRadioButton("Kode KPP (AppPortal)");

    QButtonGroup *pbkButtonGroup = new QButtonGroup(this);
    pbkButtonGroup->addButton(mNpwpTetapRadioButton);
    pbkButtonGroup->addButton(mKodeKppPbkRadioButton);

    QVBoxLayout *pbkLayout = new QVBoxLayout;
    pbkLayout->addWidget(mNpwpTetapRadioButton);
    pbkLayout->addWidget(mKodeKppPbkRadioButton);

    QGroupBox *pbkGroup = new QGroupBox("PBK");
    pbkGroup->setLayout(pbkLayout);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(pbkGroup);
    layout->addWidget(hitungBox);
    layout->addStretch();

    connect(indeksButtonGroup, SIGNAL(buttonToggled(int,bool)), SLOT(updateInput()));

    setLayout(layout);
}

void HitungPage::updateInput()
{
    mGrowthIndexLineEdit->setEnabled(mOldRadioButton->isChecked());
    mCapaianIndexLineEdit->setEnabled(mOldRadioButton->isChecked());
}

void HitungPage::save()
{
    Engine *engine = Engine::instance();

    QString rumus;
    if (mOldRadioButton->isChecked())
        rumus = "old";
    else
        rumus = "new";

    engine->databaseSettings()->setValue("hitung.rumus", rumus);

    if (rumus == "old")
    {
        engine->databaseSettings()->setValue("hitung.indeksGrowth", mGrowthIndexLineEdit->text().replace(",", ".").toDouble());
        engine->databaseSettings()->setValue("hitung.indeksCapaian", mCapaianIndexLineEdit->text().replace(",", ".").toDouble());
    }
    else
    {
        engine->databaseSettings()->remove("hitung.indeksGrowth");
        engine->databaseSettings()->remove("hitung.indeksCapaian");
    }

    engine->databaseSettings()->setValue("hitung.pbk", mKodeKppPbkRadioButton->isChecked()? 1 : 0);
}

void HitungPage::load()
{
    Engine *engine = Engine::instance();
    QString rumus = engine->databaseSettings()->value("hitung.rumus").toString();

    if (rumus == "new")
    {
        mNewRadioButton->setChecked(true);
        mGrowthIndexLineEdit->setText("0,00");
        mCapaianIndexLineEdit->setText("0,00");
    }
    else
    {
        mOldRadioButton->setChecked(true);
        mGrowthIndexLineEdit->setText(QString("%L1").arg(engine->databaseSettings()->value("hitung.indeksGrowth").toDouble(), 0, 'f', 2));
        mCapaianIndexLineEdit->setText(QString("%L1").arg(engine->databaseSettings()->value("hitung.indeksCapaian").toDouble(), 0, 'f', 2));
    }

    if (engine->databaseSettings()->value("hitung.pbk").toBool())
        mKodeKppPbkRadioButton->setChecked(true);
    else
        mNpwpTetapRadioButton->setChecked(true);
}

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent, Qt::WindowTitleHint | Qt::MSWindowsFixedSizeDialogHint)
{
    mListWidget = new QListWidget;
    mListWidget->setViewMode(QListView::IconMode);
    mListWidget->setIconSize(QSize(64,64));
    mListWidget->setMovement(QListView::Static);
    mListWidget->setMaximumWidth(72);
    mListWidget->setSpacing(4);

    QListWidgetItem *general = new QListWidgetItem(mListWidget);
    general->setIcon(QIcon(":/images/kantor.png"));
    general->setTextAlignment(Qt::AlignHCenter);
    general->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    general->setToolTip("General");

    QListWidgetItem *hitung = new QListWidgetItem(mListWidget);
    hitung->setIcon(QIcon(":/images/calc.png"));
    hitung->setTextAlignment(Qt::AlignHCenter);
    hitung->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    hitung->setToolTip("Perhitungan");

    mListWidget->setCurrentRow(0);

    mGeneralPage = new GeneralPage;
    mHitungPage = new HitungPage;

    mStackedWidget = new QStackedWidget;
    mStackedWidget->addWidget(mGeneralPage);
    mStackedWidget->addWidget(mHitungPage);

    QHBoxLayout *pagesBox = new QHBoxLayout;
    pagesBox->addWidget(mListWidget);
    pagesBox->addWidget(mStackedWidget, 1);

    QPushButton *okButton = new QPushButton("Simpan");
    QPushButton *cancelButton = new QPushButton("Cancel");

    connect(okButton, SIGNAL(clicked()), SLOT(save()));
    connect(cancelButton, SIGNAL(clicked()), SLOT(reject()));

    QHBoxLayout *buttonBox = new QHBoxLayout;
    buttonBox->addStretch();
    buttonBox->addWidget(okButton);
    buttonBox->addWidget(cancelButton);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addLayout(pagesBox);
    layout->addLayout(buttonBox);

    setLayout(layout);
    setWindowTitle("Settings");
    setWindowIcon(QIcon(":/images/settings.png"));
    resize(540, 350);

    connect(mListWidget,
            SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),
            this, SLOT(changePage(QListWidgetItem*,QListWidgetItem*)));

    load();
}

void SettingsDialog::changePage(QListWidgetItem *current, QListWidgetItem *previous)
{
    if (!current)
        current = previous;

    mStackedWidget->setCurrentIndex(mListWidget->row(current));
}

void SettingsDialog::save()
{
    mGeneralPage->save();
    mHitungPage->save();

    accept();
}

void SettingsDialog::load()
{
    mGeneralPage->load();
    mHitungPage->load();
}
