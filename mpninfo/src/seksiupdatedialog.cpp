#include "seksiupdatedialog.h"

#include <QSqlQuery>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QGridLayout>
#include <QMessageBox>

#include "engine.h"

SeksiUpdateDialog::SeksiUpdateDialog(QWidget *parent) :
    QDialog(parent, Qt::WindowTitleHint | Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint)
{
    Engine *engine = Engine::instance();

    mNameEdit = new QLineEdit;
    mSeksiComboBox = new QComboBox;
    mKodeEdit = new QLineEdit;
    mTelpEdit = new QLineEdit;

    mKodeEdit->setAlignment(Qt::AlignCenter);
    mKodeEdit->setMaximumWidth(30);
    mKodeEdit->setInputMask("00;_");

    mTelpEdit->setMaximumWidth(200);

    QPushButton *okButton = new QPushButton("Ok");
    QPushButton *cancelButton = new QPushButton("Batal");

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch();
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    QStringList seksiList = engine->common()->namaJenisSeksiList();
    for (int i=0; i<seksiList.count(); i++)
        mSeksiComboBox->addItem(seksiList[i], i);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(new QLabel("Seksi"), 0, 0);
    layout->addWidget(new QLabel("Nama"), 1, 0);
    layout->addWidget(new QLabel("Kode"), 2, 0);
    layout->addWidget(new QLabel("Telepon"), 3, 0);
    layout->addWidget(mSeksiComboBox, 0, 1);
    layout->addWidget(mNameEdit, 1, 1);
    layout->addWidget(mKodeEdit, 2, 1);
    layout->addWidget(mTelpEdit, 3, 1);
    layout->addLayout(buttonLayout, 4, 0, 1, 2);

    setLayout(layout);
    setWindowTitle("Add / Update Seksi");

    connect(mSeksiComboBox, SIGNAL(currentIndexChanged(int)), SLOT(seksiSelected()));
    connect(okButton, SIGNAL(clicked()), SLOT(checkValues()));
    connect(cancelButton, SIGNAL(clicked()), SLOT(reject()));

    seksiSelected();
}

void SeksiUpdateDialog::checkValues()
{
    if (mNameEdit->text().isEmpty()) {
        QMessageBox::warning(this, "Error", "Semua isian tidak boleh kosong.");
        return;
    }

    accept();
}

void SeksiUpdateDialog::seksiSelected()
{
    QString nama = mSeksiComboBox->currentText();
    mNameEdit->setText(nama);
}

void SeksiUpdateDialog::setSeksi(const QString &kantor, int id)
{
    Engine *engine = Engine::instance();
    Seksi seksi = engine->data()->seksi(kantor, id);

   mSeksiComboBox->setCurrentIndex(mSeksiComboBox->findData(seksi.type));
   mNameEdit->setText(seksi.nama);
   mKodeEdit->setText(seksi.kode);
   mTelpEdit->setText(seksi.telp);
}

int SeksiUpdateDialog::seksi()
{
    return mSeksiComboBox->itemData(mSeksiComboBox->currentIndex()).toInt();
}

QString SeksiUpdateDialog::name()
{
    return mNameEdit->text();
}

QString SeksiUpdateDialog::telepon()
{
    return mTelpEdit->text();
}

QString SeksiUpdateDialog::kode()
{
    return mKodeEdit->text();
}
