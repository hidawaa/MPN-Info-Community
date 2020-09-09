#include "mapsearchdialog.h"

#include <QGridLayout>
#include <QLineEdit>
#include <QStandardItemModel>
#include <QSqlQuery>
#include <QTreeView>
#include <QHeaderView>
#include <QLabel>

#include "engine.h"

MapSearchDialog::MapSearchDialog(QWidget *parent) :
    QDialog(parent)
{
    mResultView = new QTreeView;
    mKeywordEdit = new QLineEdit;

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(new QLabel("Search"), 0, 0);
    layout->addWidget(mKeywordEdit, 0, 1);
    layout->addWidget(mResultView, 1, 0, 1, 2);

    setLayout(layout);
    setWindowTitle("Referensi - MAP");
    resize(500, 500);

    connect(mKeywordEdit, SIGNAL(textChanged(QString)), SLOT(search()));
    search();
}

void MapSearchDialog::search()
{
    QString keyword = mKeywordEdit->text();

    delete mResultView->model();

    QStandardItemModel *model = new QStandardItemModel(0, 4, mResultView);
    model->setHeaderData(0, Qt::Horizontal, "KdMap");
    model->setHeaderData(1, Qt::Horizontal, "KdBayar");
    model->setHeaderData(2, Qt::Horizontal, "Uraian");
    model->setHeaderData(3, Qt::Horizontal, "Sektor");


    Engine *engine = Engine::instance();
    DatabasePtr db = engine->database();

    QString searchSql = QString("SELECT * FROM `map` WHERE (`kdmap` LIKE '%1%' OR `uraian` LIKE '%%1%') AND `kdbayar`!='000' ORDER BY `kdmap`, `kdbayar`").arg(keyword);
    db->exec(searchSql);

    while (db->next()) {
        QString kdmap = db->value(0).toString();
        QString kdbayar = db->value(1).toString();
        int sektor = db->value(2).toInt();
        QString uraian = db->value(3).toString();

        QStandardItem *kdmapItem = new QStandardItem(kdmap);
        kdmapItem->setTextAlignment(Qt::AlignCenter);

        QStandardItem *kdbayarItem = new QStandardItem(kdbayar);
        kdbayarItem->setTextAlignment(Qt::AlignCenter);

        QStandardItem *uraianItem = new QStandardItem(uraian);
        QStandardItem *sektorItem = new QStandardItem(engine->common()->namaSektor(sektor));

        QList<QStandardItem *> itemList;
        itemList << kdmapItem << kdbayarItem << uraianItem << sektorItem;

        model->appendRow(itemList);
    }

    mResultView->setModel(model);
    mResultView->setColumnWidth(0, 50);
    mResultView->setColumnWidth(1, 30);
    mResultView->setColumnWidth(2, 350);
    mResultView->setColumnWidth(3, 150);
}
