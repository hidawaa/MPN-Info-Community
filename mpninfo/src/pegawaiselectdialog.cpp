#include "pegawaiselectdialog.h"

#include <QDate>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QTableView>
#include <QComboBox>
#include <QLabel>
#include <QStandardItemModel>

#include "engine.h"

PegawaiSelectDialog::PegawaiSelectDialog(QWidget *parent) :
    QDialog(parent, Qt::WindowTitleHint | Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint),
    mJabatan(JabatanUnknown)
{
    Engine *engine = Engine::instance();

    mPegawaiView = new QTableView;
    mPegawaiView->setAlternatingRowColors(true);
    mPegawaiView->setShowGrid(false);
    mPegawaiView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mPegawaiView->setSelectionBehavior(QAbstractItemView::SelectRows);
    mPegawaiView->setSelectionMode(QAbstractItemView::SingleSelection);
    mPegawaiView->horizontalHeader()->setStretchLastSection(true);
    mPegawaiView->horizontalHeader()->setHighlightSections(false);
    mPegawaiView->verticalHeader()->hide();
    mPegawaiView->verticalHeader()->setDefaultSectionSize(fontMetrics().height()+8);

    mKantorComboBox = new QComboBox;
    QString kodeKantor = engine->databaseSettings()->value(IDS_SERVER_KANTOR_KODE).toString();
    foreach (const Kantor &kantor, engine->data()->kppList(kodeKantor))
        mKantorComboBox->addItem(kantor.kode + " - " + kantor.nama, kantor.kode);

    QHBoxLayout *filterLayout = new QHBoxLayout;
    filterLayout->addWidget(new QLabel("Kantor"));
    filterLayout->addWidget(mKantorComboBox);
    filterLayout->addStretch();

    mOkButton = new QPushButton("Pilih");
    QPushButton *cancelButton = new QPushButton("Batal");
    mOkButton->setEnabled(false);

    QHBoxLayout *buttonBox = new QHBoxLayout;
    buttonBox->addStretch();
    buttonBox->addWidget(mOkButton);
    buttonBox->addWidget(cancelButton);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addLayout(filterLayout);
    layout->addWidget(mPegawaiView);
    layout->addLayout(buttonBox);

    setLayout(layout);
    setWindowTitle("Pilih Pegawai");
    resize(500, 300);

    connect(mKantorComboBox, SIGNAL(currentIndexChanged(int)), SLOT(updateModel()));
    connect(mOkButton, SIGNAL(clicked()), SLOT(accept()));
    connect(cancelButton, SIGNAL(clicked()), SLOT(reject()));
    connect(mPegawaiView, SIGNAL(clicked(QModelIndex)), SLOT(updateButton()));

    updateModel();
}

QString PegawaiSelectDialog::kodeKantor()
{
    return mKantorComboBox->currentData().toString();
}

void PegawaiSelectDialog::setOnlyShowCurrentKantor(bool value)
{
    Engine *engine = Engine::instance();
    QString kodeKantor = engine->databaseSettings()->value(IDS_SERVER_KANTOR_KODE).toString();

    mKantorComboBox->clear();
    foreach (const Kantor &kantor, engine->data()->kppList(kodeKantor))
        if (value && (kantor.kode == kodeKantor))
            mKantorComboBox->addItem(kantor.nama, kantor.kode);
}

void PegawaiSelectDialog::setOnlyShowKantor(const QString &kodeKpp)
{
    Engine *engine = Engine::instance();
    mKantorComboBox->clear();
    foreach (const Kantor &kantor, engine->data()->kppList(kodeKpp))
        if (kantor.kode == kodeKpp)
            mKantorComboBox->addItem(kantor.nama, kantor.kode);
}

void PegawaiSelectDialog::setFilterByJabatan(int jabatan)
{
    mJabatan = jabatan;
    updateModel();
}

void PegawaiSelectDialog::updateModel()
{
    Engine *engine = Engine::instance();
    QList<Pegawai> pegawaiList = engine->data()->pegawaiList(mKantorComboBox->currentData().toString(), QDate::currentDate().year());

    delete mPegawaiView->model();
    QStandardItemModel *model = new QStandardItemModel(0, 3, mPegawaiView);
    model->setHeaderData(0, Qt::Horizontal, "NIP");
    model->setHeaderData(1, Qt::Horizontal, "Nama");
    model->setHeaderData(2, Qt::Horizontal, "Jabatan");

    foreach (const Pegawai &pegawai, pegawaiList) {
        if (mJabatan != JabatanUnknown && pegawai.jabatan.type != mJabatan)
            continue;

        QStandardItem *nipItem = new QStandardItem(pegawai.nip);
        nipItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);

        QStandardItem *nameItem = new QStandardItem(pegawai.nama);

        QStandardItem *jabatanItem = new QStandardItem(engine->common()->namaJabatan(pegawai.jabatan.type));
        jabatanItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);

        QList<QStandardItem *> itemList;
        itemList << nipItem << nameItem << jabatanItem;
        model->appendRow(itemList);
    }

    mPegawaiView->setModel(model);

    mPegawaiView->setColumnWidth(0, 70);
    mPegawaiView->setColumnWidth(1, 220);
}

void PegawaiSelectDialog::updateButton()
{
    int row = mPegawaiView->currentIndex().row();
    QStandardItemModel *model = static_cast<QStandardItemModel *>(mPegawaiView->model());

    mSelectedPegawaiNip = model->item(row, 0)->text();
    if (mSelectedPegawaiNip.isEmpty())
        return;

    mOkButton->setEnabled(true);
}
