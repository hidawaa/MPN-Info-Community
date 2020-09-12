#include "klusearchdialog.h"

#include <QGridLayout>
#include <QLineEdit>
#include <QStandardItemModel>
#include <QSqlQuery>
#include <QHeaderView>
#include <QTreeView>
#include <QLabel>

#include "engine.h"

KluSearchDialog::KluSearchDialog(QWidget *parent) :
    QDialog(parent)
{
    mResultView = new QTreeView;
    mKeywordEdit = new QLineEdit;

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(new QLabel("Search"), 0, 0);
    layout->addWidget(mKeywordEdit, 0, 1);
    layout->addWidget(mResultView, 1, 0, 1, 2);

    setLayout(layout);
    setWindowTitle("Referensi - KLU");
    resize(800, 500);

    connect(mKeywordEdit, SIGNAL(textChanged(QString)), SLOT(search()));
    search();
}

void KluSearchDialog::search()
{
    delete mResultView->model();

    QStandardItemModel *model = new QStandardItemModel(0, 3, mResultView);
    model->setHeaderData(0, Qt::Horizontal, "KLU");
    model->setHeaderData(1, Qt::Horizontal, "Uraian");
    model->setHeaderData(2, Qt::Horizontal, "Sektor");

    Engine *engine = Engine::instance();
    DatabasePtr db = engine->database();

    QString keyword = mKeywordEdit->text();
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
    mResultView->setColumnWidth(0, 100);
    mResultView->setColumnWidth(1, 400);
    mResultView->setColumnWidth(2, 200);
}
