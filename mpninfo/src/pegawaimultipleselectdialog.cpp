#include "pegawaimultipleselectdialog.h"

#include <QDate>
#include <QTableView>
#include <QStandardItemModel>
#include <QPushButton>
#include <QLabel>
#include <QGridLayout>
#include <QComboBox>
#include <QSqlQuery>

#include "checkedheaderview.h"

#include "engine.h"

PegawaiMultipleSelectDialog::PegawaiMultipleSelectDialog(QWidget *parent) :
    QDialog(parent)
{
    Engine *engine = Engine::instance();

    mResultView = new QTableView;
    QFont font = mResultView->font();
    font.setPointSize(8);

    CheckedHeaderView *headerView = new CheckedHeaderView(Qt::Horizontal);
    headerView->setSectionsClickable(true);
    mResultView->setHorizontalHeader(headerView);

    mResultView->setFont(font);
    mResultView->setMinimumWidth(500);
    mResultView->setMinimumHeight(400);
    mResultView->setAlternatingRowColors(true);
    mResultView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mResultView->setSelectionBehavior(QAbstractItemView::SelectRows);
    mResultView->setSelectionMode(QAbstractItemView::SingleSelection);
    mResultView->setWordWrap(false);
    mResultView->horizontalHeader()->setStretchLastSection(true);
    mResultView->horizontalHeader()->setHighlightSections(false);
    mResultView->verticalHeader()->hide();
    mResultView->verticalHeader()->setDefaultSectionSize(fontMetrics().height()+10);

    mKantorComboBox = new QComboBox;
    QString kodeKantor = engine->databaseSettings()->value(IDS_SERVER_KANTOR_KODE).toString();
    foreach (const Kantor &kantor, engine->data()->kppList(kodeKantor))
        mKantorComboBox->addItem(kantor.nama, kantor.kode);

    mTahunComboBox = new QComboBox;

    int currentYear = QDate::currentDate().year();
    for (int i=currentYear; i >= currentYear - 5; i--)
        mTahunComboBox->addItem(QString::number(i), i);

    QHBoxLayout *tahunLayout = new QHBoxLayout;
    tahunLayout->addWidget(new QLabel("Kantor"));
    tahunLayout->addWidget(mKantorComboBox);
    tahunLayout->addWidget(new QLabel("Tahun"));
    tahunLayout->addWidget(mTahunComboBox);
    tahunLayout->addStretch();

    QPushButton *okButton = new QPushButton("Ok");
    QPushButton *cancelButton = new QPushButton("Batal");

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch();
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addLayout(tahunLayout);
    layout->addWidget(mResultView);
    layout->addLayout(buttonLayout);

    setLayout(layout);
    setWindowTitle("Select Multiple Pegawai");

    connect(okButton, SIGNAL(clicked()), SLOT(accept()));
    connect(cancelButton, SIGNAL(clicked()), SLOT(reject()));
    connect(headerView, SIGNAL(toggled(bool)), SLOT(checkAll(bool)));

    connect(mKantorComboBox, SIGNAL(currentIndexChanged(int)), SLOT(updateResult()));
    connect(mTahunComboBox, SIGNAL(currentIndexChanged(int)), SLOT(updateResult()));

    updateResult();
    resize(700, 400);
}

void PegawaiMultipleSelectDialog::checkAll(bool toggle)
{
    QStandardItemModel *model = static_cast<QStandardItemModel *>(mResultView->model());
    for (int i=0; i < model->rowCount(); i++)
        model->item(i, 0)->setCheckState(toggle? Qt::Checked : Qt::Unchecked);
}

QString PegawaiMultipleSelectDialog::kodeKantor()
{
    return mKantorComboBox->currentData().toString();
}

int PegawaiMultipleSelectDialog::tahun()
{
    return mTahunComboBox->currentData().toInt();
}

QStringList PegawaiMultipleSelectDialog::nipList()
{
    QStandardItemModel *model = static_cast<QStandardItemModel *>(mResultView->model());

    QStringList nipList;
    for (int i=0; i < model->rowCount(); i++) {
        if (model->item(i, 0)->checkState() == Qt::Checked)
            nipList << model->item(i, 1)->text();
    }

    return nipList;
}

void PegawaiMultipleSelectDialog::updateResult()
{
    delete mResultView->model();

    Engine *engine = Engine::instance();

    QStandardItemModel *model = new QStandardItemModel(0, 5, mResultView);
    model->setHeaderData(1, Qt::Horizontal, "NIP");
    model->setHeaderData(2, Qt::Horizontal, "Nama");
    model->setHeaderData(3, Qt::Horizontal, "Seksi");
    model->setHeaderData(4, Qt::Horizontal, "Jabatan");

    QString pegawaiSql = QString("SELECT `nip`, `nama`, `seksi`, `jabatan` FROM `pegawai` WHERE `tahun`='%1' AND `kantor`='%2' ORDER BY `seksi`, `jabatan`").arg(tahun()).arg(kodeKantor());
    DatabasePtr db = engine->database();
    db->exec(pegawaiSql);

    while (db->next()) {
        QString nip = db->value(0).toString();
        QString nama = db->value(1).toString();
        int seksi = db->value(2).toInt();
        int jabatan = db->value(3).toInt();

        QStandardItem *checkItem = new QStandardItem;
        checkItem->setCheckable(true);

        QStandardItem *nipItem = new QStandardItem(nip);
        nipItem->setTextAlignment(Qt::AlignCenter);

        QStandardItem *namaItem = new QStandardItem(nama);

        QStandardItem *seksiItem = new QStandardItem(engine->data()->seksi(mKantorComboBox->currentData().toString(), seksi).nama);
        seksiItem->setTextAlignment(Qt::AlignCenter);

        QStandardItem *jabatanItem = new QStandardItem(engine->common()->namaJabatan(jabatan));
        jabatanItem->setTextAlignment(Qt::AlignCenter);


        QList<QStandardItem *> itemList;
        itemList << checkItem << nipItem << namaItem << seksiItem << jabatanItem;

        model->appendRow(itemList);
    }

    mResultView->setModel(model);
    mResultView->setColumnWidth(0, 30);
    mResultView->setColumnWidth(1, 70);
    mResultView->setColumnWidth(2, 200);
    mResultView->setColumnWidth(3, 200);
}
