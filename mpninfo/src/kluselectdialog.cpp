#include "kluselectdialog.h"

#include <QGridLayout>
#include <QLineEdit>
#include <QStandardItemModel>
#include <QSqlQuery>
#include <QHeaderView>
#include <QLabel>
#include <QTreeView>
#include <QPushButton>

#include "engine.h"

KluSelectDialog::KluSelectDialog(QWidget *parent) :
    QDialog(parent)
{
    mResultView = new QTreeView;
    mResultView->setAlternatingRowColors(true);
    mResultView->setRootIsDecorated(false);
    mResultView->setSortingEnabled(true);
    mResultView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mResultView->setSelectionBehavior(QAbstractItemView::SelectRows);
    mResultView->header()->setStretchLastSection(false);
    mResultView->header()->setHighlightSections(false);

    mKeywordEdit = new QLineEdit;

    QPushButton *okButton = new QPushButton("Pilih");
    QPushButton *cancelButton = new QPushButton("Batal");

    QVBoxLayout *buttonLayout = new QVBoxLayout;
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);
    buttonLayout->addStretch();

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(new QLabel("Search"), 0, 0);
    layout->addWidget(mKeywordEdit, 0, 1);
    layout->addWidget(mResultView, 1, 0, 1, 2);
    layout->addLayout(buttonLayout, 0, 2, 2, 1);

    setLayout(layout);
    setWindowTitle("KLU");
    resize(500, 500);

    connect(okButton, SIGNAL(clicked()), SLOT(accept()));
    connect(cancelButton, SIGNAL(clicked()), SLOT(reject()));
    connect(mKeywordEdit, SIGNAL(textChanged(QString)), SLOT(search()));
    search();
}

QString KluSelectDialog::klu()
{
    int row = mResultView->currentIndex().row();
    return mResultView->model()->index(row, 0).data().toString();
}

void KluSelectDialog::search()
{
    QString keyword = mKeywordEdit->text();

    delete mResultView->model();

    QStandardItemModel *model = new QStandardItemModel(0, 3, mResultView);
    model->setHeaderData(0, Qt::Horizontal, "KLU");
    model->setHeaderData(1, Qt::Horizontal, "Uraian");
    model->setHeaderData(2, Qt::Horizontal, "Sektor");

    Engine *engine = Engine::instance();
    DatabasePtr db = engine->database();
    QString searchSql = QString("SELECT * FROM `klu` WHERE `kode` LIKE '%1%' OR `nama` LIKE '%%1%'").arg(keyword);
    db->exec(searchSql);
    while (db->next()) {
        QString kode = db->value(0).toString();
        QString uraian = db->value(1).toString();
        QString sektor = db->value(2).toString();

        QStandardItem *kluItem = new QStandardItem(kode);
        kluItem->setTextAlignment(Qt::AlignCenter);

        QStandardItem *uraianItem = new QStandardItem(uraian);
        QStandardItem *sektorItem = new QStandardItem(engine->common()->namaSektor(sektor));

        QList<QStandardItem *> itemList;
        itemList << kluItem << uraianItem << sektorItem;

        model->appendRow(itemList);
    }

    mResultView->setModel(model);
    mResultView->setColumnWidth(0, 50);
    mResultView->setColumnWidth(1, 400);
    mResultView->setColumnWidth(2, 200);
}
