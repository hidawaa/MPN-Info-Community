#include "seksimanagedialog.h"

#include <QSqlQuery>
#include <QTableView>
#include <QHeaderView>
#include <QMessageBox>
#include <QStandardItemModel>
#include <QGridLayout>
#include <QPushButton>
#include <QComboBox>
#include <QLabel>

#include "engine.h"
#include "cdata.h"
#include "seksiupdatedialog.h"

SeksiManageDialog::SeksiManageDialog(QWidget *parent) :
    QDialog(parent)
{
    Engine *engine = Engine::instance();

    mResultView = new QTableView;
    mResultView->setAlternatingRowColors(true);
    mResultView->setShowGrid(false);
    mResultView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mResultView->setSelectionBehavior(QAbstractItemView::SelectRows);
    mResultView->setSelectionMode(QAbstractItemView::SingleSelection);
    mResultView->horizontalHeader()->setStretchLastSection(true);
    mResultView->horizontalHeader()->setHighlightSections(false);
    mResultView->verticalHeader()->hide();
    mResultView->verticalHeader()->setDefaultSectionSize(fontMetrics().height()+8);

    mKantorComboBox = new QComboBox;
    QString kodeKantor = engine->databaseSettings()->value(IDS_SERVER_KANTOR_KODE).toString();
    foreach (const Kantor &kantor, engine->data()->kppList(kodeKantor))
            mKantorComboBox->addItem(kantor.nama, kantor.kode);

    QHBoxLayout *filterLayout = new QHBoxLayout;
    filterLayout->addWidget(new QLabel("Kantor"));
    filterLayout->addWidget(mKantorComboBox);
    filterLayout->addStretch();

    QPushButton *okButton = new QPushButton(tr("&Ok"));
    mAddButton = new QPushButton(tr("&Tambah"));
    mDelButton = new QPushButton(tr("&Delete"));
    mEditButton = new QPushButton(tr("&Ubah"));

    mDelButton->setEnabled(false);
    mEditButton->setEnabled(false);

    QVBoxLayout *buttonBox = new QVBoxLayout;
    buttonBox->addWidget(okButton);
    buttonBox->addSpacing(10);
    buttonBox->addWidget(mAddButton);
    buttonBox->addWidget(mDelButton);
    buttonBox->addWidget(mEditButton);
    buttonBox->addStretch();

    QGridLayout *layout = new QGridLayout;
    layout->addLayout(filterLayout, 0, 0, 1, 2);
    layout->addWidget(mResultView, 1, 0);
    layout->addLayout(buttonBox, 1, 1);

    setLayout(layout);
    setWindowTitle("Manage Seksi");
    setWindowIcon(QIcon(":/images/pegawai.png"));
    resize(QSize(580, 450));

    connect(mKantorComboBox, SIGNAL(currentIndexChanged(int)), SLOT(updateModel()));
    connect(okButton, SIGNAL(clicked()), SLOT(accept()));
    connect(mAddButton, SIGNAL(clicked()), SLOT(addSeksi()));
    connect(mDelButton, SIGNAL(clicked()), SLOT(deleteSeksi()));
    connect(mEditButton, SIGNAL(clicked()), SLOT(editSeksi()));
    connect(mResultView, SIGNAL(clicked(QModelIndex)), SLOT(updateButton()));

    updateModel();
}

void SeksiManageDialog::updateModel()
{
    delete mResultView->model();

    QStandardItemModel *model = new QStandardItemModel(0, 3, mResultView);
    model->setHeaderData(0, Qt::Horizontal, "id");
    model->setHeaderData(1, Qt::Horizontal, "Nama");
    model->setHeaderData(2, Qt::Horizontal, "Tipe");

    QString seksiSql = QString("SELECT * FROM `seksi` WHERE `kantor`='%1' ORDER BY `id`")
            .arg(mKantorComboBox->currentData().toString());

    Engine *engine = Engine::instance();
    DatabasePtr db = engine->database();
    db->exec(seksiSql);

    while (db->next()) {
        int id = db->value(0).toInt();
        int tipe = db->value(2).toInt();
        QString nama = db->value(3).toString();

        QStandardItem *idItem = new QStandardItem;
        idItem->setData(id, Qt::DisplayRole);

        QStandardItem *namaItem = new QStandardItem(nama);
        QStandardItem *tipeItem = new QStandardItem(engine->common()->namaJenisSeksi(tipe));
        tipeItem->setTextAlignment(Qt::AlignCenter);

        QList<QStandardItem *> itemList;
        itemList << idItem << namaItem << tipeItem;

        model->appendRow(itemList);
    }

    mResultView->setModel(model);
    mResultView->hideColumn(0);
    mResultView->setColumnWidth(1, 250);
}

void SeksiManageDialog::updateButton()
{
    int row = mResultView->currentIndex().row();
    QStandardItemModel *model = static_cast<QStandardItemModel *>(mResultView->model());

    int tempSeksiId = model->item(row, 0)->text().toInt();
    mDelButton->setEnabled(true);
    mEditButton->setEnabled(true);

    mSelectedSeksiId = tempSeksiId;
}

void SeksiManageDialog::addSeksi()
{
    SeksiUpdateDialog dialog(this);
    if (dialog.exec() == QDialog::Rejected)
        return;

    Engine *engine = Engine::instance();
    if (static_cast<CData *>(engine->data())->addSeksi(mKantorComboBox->currentData().toString(), dialog.seksi(), dialog.name(), dialog.kode(), dialog.telepon()))
        QMessageBox::information(this, "Add Berhasil", "Seksi berhasil ditambahkan.");
    else
        QMessageBox::warning(this, "Add Gagal", "Seksi gagal ditambahkan.");

    updateModel();
}

void SeksiManageDialog::editSeksi()
{
    SeksiUpdateDialog dialog(this);
    dialog.setSeksi(mKantorComboBox->currentData().toString(), mSelectedSeksiId);
    if (dialog.exec() == QDialog::Rejected)
        return;

    Engine *engine = Engine::instance();
    if (static_cast<CData *>(engine->data())->updateSeksi(mSelectedSeksiId, mKantorComboBox->currentData().toString(), dialog.seksi(), dialog.name(), dialog.kode(), dialog.telepon()))
        QMessageBox::information(this, "Edit Berhasil", "Seksi berhasil diubah.");
    else
        QMessageBox::warning(this, "Edit Gagal", "Seksi gagal diubah.");

    updateModel();
}

void SeksiManageDialog::deleteSeksi()
{
    int result = QMessageBox::question(this, "Hapus Seksi", "Apakah Anda yakin?", QMessageBox::Yes | QMessageBox::No);
    if (result == QMessageBox::No)
        return;

    Engine *engine = Engine::instance();
    if (static_cast<CData *>(engine->data())->removeSeksi(mSelectedSeksiId, mKantorComboBox->currentData().toString()))
        QMessageBox::information(this, "Hapus Berhasil", "Seksi berhasil dihapus.");
    else
        QMessageBox::warning(this, "Hapus Gagal", "Seksi gagal dihapus.");

    updateModel();
}
