#include "pegawaimanagedialog.h"

#include <QDate>
#include <QPushButton>
#include <QTableView>
#include <QHeaderView>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QStandardItemModel>
#include <QComboBox>
#include <QLabel>
#include <QMenu>
#include <QAction>
#include <QMenuBar>
#include <QFileDialog>

#include "engine.h"
#include "cdata.h"
#include "pegawaiupdatedialog.h"
#include "pegawaiselectdialog.h"
#include "pegawaimultipleselectdialog.h"

PegawaiManageDialog::PegawaiManageDialog(QWidget *parent) :
    QDialog(parent, Qt::WindowTitleHint | Qt::WindowCloseButtonHint)
{
    Engine *engine = Engine::instance();

    mPegawaiView = new QTableView;
    mPegawaiView->setSortingEnabled(true);
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

    Kantor kantor = engine->data()->kantor(kodeKantor);
    mKantorComboBox->addItem(kantor.nama, kantor.kode);

    foreach (const Kantor &kpp, engine->data()->kppList(kodeKantor))
        mKantorComboBox->addItem(kpp.nama, kpp.kode);

    int currentYear = QDate::currentDate().year();

    mTahunComboBox = new QComboBox;
    for (int i=currentYear; i > currentYear-5; i--)
        mTahunComboBox->addItem(QString::number(i), i);

    QHBoxLayout *tahunLayout = new QHBoxLayout;
    tahunLayout->addWidget(new QLabel("Kantor"));
    tahunLayout->addWidget(mKantorComboBox);
    tahunLayout->addWidget(new QLabel("Tahun"));
    tahunLayout->addWidget(mTahunComboBox);
    tahunLayout->addStretch();

    mOkButton = new QPushButton(tr("&Ok"));
    mAddButton = new QPushButton(tr("&Tambah"));
    mDelButton = new QPushButton(tr("&Delete"));
    mEditButton = new QPushButton(tr("&Ubah"));
    QPushButton *copyButton = new QPushButton("&Copy");

    QVBoxLayout *buttonBox = new QVBoxLayout;
    buttonBox->addWidget(mOkButton);
    buttonBox->addSpacing(10);
    buttonBox->addWidget(mAddButton);
    buttonBox->addWidget(mDelButton);
    buttonBox->addWidget(mEditButton);
    buttonBox->addSpacing(10);
    buttonBox->addWidget(copyButton);
    buttonBox->addStretch();

    QGridLayout *layout = new QGridLayout;
    layout->addLayout(tahunLayout, 0, 0);
    layout->addWidget(mPegawaiView, 1, 0);
    layout->addLayout(buttonBox, 1, 1);

    setLayout(layout);
    setWindowTitle("Pengaturan Pegawai");
    setWindowIcon(QIcon(":/images/pegawai.png"));
    resize(QSize(800, 500));

    connect(mKantorComboBox, SIGNAL(currentIndexChanged(int)), SLOT(updateModel()));
    connect(mTahunComboBox, SIGNAL(currentIndexChanged(int)), SLOT(updateModel()));
    connect(mOkButton, SIGNAL(clicked()), SLOT(accept()));
    connect(mAddButton, SIGNAL(clicked()), SLOT(addPegawai()));
    connect(mEditButton, SIGNAL(clicked()), SLOT(editPegawai()));
    connect(mDelButton, SIGNAL(clicked()), SLOT(deletePegawai()));
    connect(copyButton, SIGNAL(clicked()), SLOT(copyPegawai()));
    connect(mPegawaiView, SIGNAL(clicked(QModelIndex)), SLOT(updateButton()));

    updateModel();
}

void PegawaiManageDialog::reset()
{
    mSelectedPegawaiNip.clear();
    mDelButton->setEnabled(false);
    mEditButton->setEnabled(false);
}

void PegawaiManageDialog::updateModel()
{
    Engine *engine = Engine::instance();
    QList<Pegawai> pegawaiList = engine->data()->pegawaiList(mKantorComboBox->currentData().toString(), mTahunComboBox->currentData().toInt());

    delete mPegawaiView->model();
    QStandardItemModel *model = new QStandardItemModel(pegawaiList.count(), 4, mPegawaiView);
    model->setHeaderData(0, Qt::Horizontal, "NIP");
    model->setHeaderData(1, Qt::Horizontal, "Nama");
    model->setHeaderData(2, Qt::Horizontal, "Seksi");
    model->setHeaderData(3, Qt::Horizontal, "Jabatan");

    int row = 0;
    foreach (const Pegawai &pegawai, pegawaiList) {
        QStandardItem *nipItem = new QStandardItem(pegawai.nip);
        nipItem->setTextAlignment(Qt::AlignCenter);
        model->setItem(row, 0, nipItem);

        QStandardItem *nameItem = new QStandardItem(pegawai.nama);
        model->setItem(row, 1, nameItem);

        QStandardItem *seksiItem = new QStandardItem(pegawai.seksi.nama);
        seksiItem->setTextAlignment(Qt::AlignCenter);
        model->setItem(row, 2, seksiItem);

        QStandardItem *jabatanItem = new QStandardItem(engine->common()->namaJabatan(pegawai.jabatan.type));
        jabatanItem->setTextAlignment(Qt::AlignCenter);
        model->setItem(row, 3, jabatanItem);

        row++;
    }

    mPegawaiView->setModel(model);

    mPegawaiView->setColumnWidth(0, 70);
    mPegawaiView->setColumnWidth(1, 200);
    mPegawaiView->setColumnWidth(2, 200);

    reset();
}

void PegawaiManageDialog::updateButton()
{
    int row = mPegawaiView->currentIndex().row();
    QStandardItemModel *model = static_cast<QStandardItemModel *>(mPegawaiView->model());

    QString tempPegawaiNip =  model->item(row, 0)->text();
    if (tempPegawaiNip.isEmpty())
        return;

    mDelButton->setEnabled(true);
    mEditButton->setEnabled(true);

    mSelectedPegawaiNip = tempPegawaiNip;
}

void PegawaiManageDialog::addPegawai()
{
    Engine *engine = Engine::instance();

    PegawaiUpdateDialog dialog;
    dialog.setKantor(mKantorComboBox->currentData().toString());

    bool done = false;
    while ( !done ) {
        if ( dialog.exec() == QDialog::Accepted ) {
            if ( dialog.nip().isEmpty()
                 || dialog.nip2().isEmpty()
                 || dialog.nama().isEmpty()
                 || dialog.pangkat() == -1
                 || dialog.jabatan() == -1 ) {
                QMessageBox::information(this, "Info", "Isian tidak boleh kosong.");
            }
            else {
                if (static_cast<CData *>(engine->data())->addPegawai(mKantorComboBox->currentData().toString(), dialog.nip(), dialog.nip2(), dialog.nama(), dialog.seksi(), dialog.pangkat(), dialog.jabatan(), dialog.plh(), mTahunComboBox->currentData().toInt()) ) {
                    updateModel();
                    QMessageBox::information(this, "Info", "Data Pegawai berhasil ditambahkan.");
                    done = true;
                }
                else {
                    QMessageBox::warning(this, "Error", "Data Pegawai gagal ditambahkan.");
                }
            }
        }
        else {
            done =  true;
        }
    }
}

void PegawaiManageDialog::editPegawai()
{
    Engine *engine = Engine::instance();

    QString kodeKantor = mKantorComboBox->currentData().toString();
    int tahun = mTahunComboBox->currentData().toInt();

    PegawaiUpdateDialog dialog;
    dialog.setMode(PegawaiUpdateDialog::EditMode);
    dialog.setKantor(mKantorComboBox->currentData().toString());
    dialog.setNipPegawai(kodeKantor, mSelectedPegawaiNip, tahun);
    if ( dialog.exec() == QDialog::Accepted ) {
        if (static_cast<CData *>(engine->data())->updatePegawai(kodeKantor, dialog.nip(), dialog.nip2(), dialog.nama(), dialog.seksi(), dialog.pangkat(), dialog.jabatan(), dialog.plh(), tahun)) {
            updateModel();
            QMessageBox::information(this, "Info", "Data Pegawai berhasil diubah.");
        }
        else {
            QMessageBox::warning(this, "Error", "Data Pegawai gagal diubah.");
        }
    }
}

void PegawaiManageDialog::deletePegawai()
{
    Engine *engine = Engine::instance();
    QString nip = mSelectedPegawaiNip;

    if ( QMessageBox::question(this, "Konfirmasi", "Apakah Anda yakin akan menghapus data pegawai ini?", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes ) {
        QString kantor = mKantorComboBox->currentData().toString();
        int tahun = mTahunComboBox->currentData().toInt();
        if (static_cast<CData *>(engine->data())->deletePegawai(kantor, nip, tahun) ) {
            updateModel();
            QMessageBox::information(this, "Info", "Data Pegawai berhasil dihapus.");

            if (tahun != QDate::currentDate().year())
                return;

            CData *data = static_cast<CData *>(engine->data());
            if (!data->user(nip).username.isEmpty()) {
                if ( QMessageBox::question(this, "Konfirmasi", "Apakah Anda ingin menghapus User login pegawai bersangkutan?", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes ) {
                    if (data->deleteUser(nip))
                        QMessageBox::information(this, "Info", "Data User berhasil dihapus.");
                    else
                        QMessageBox::warning(this, "Error", "Data User gagal dihapus.");
                }
            }
        }
        else {
            QMessageBox::warning(this, "Error", "Data Pegawai gagal dihapus.");
        }
    }
}

void PegawaiManageDialog::copyPegawai()
{
    Engine *engine = Engine::instance();

    PegawaiMultipleSelectDialog dialog(this);
    if (!dialog.exec())
        return;

    int totalSuccess = 0;
    foreach (const QString &nip, dialog.nipList()) {
        Pegawai pegawai = engine->data()->pegawai(dialog.kodeKantor(), dialog.tahun(), nip);

        CData *data = static_cast<CData *>(engine->data());
        if (data->addPegawai(mKantorComboBox->currentData().toString(), pegawai.nip, pegawai.nip2, pegawai.nama, pegawai.seksi.type, pegawai.pangkat.type, pegawai.jabatan.type, pegawai.plh, mTahunComboBox->currentData().toInt()) ) {
            updateModel();
            totalSuccess++;
        }
    }

    if (totalSuccess > 0) {
        QMessageBox::information(this, "Copy Pegawai", "Sebanyak " + QString::number(totalSuccess) + " pegawai berhasil ditambahkan");
    }
}
