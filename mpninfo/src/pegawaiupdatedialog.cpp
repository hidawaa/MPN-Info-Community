#include "pegawaiupdatedialog.h"

#include <QSqlQuery>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QMessageBox>
#include <QToolButton>

#include "engine.h"
#include "cdata.h"

#include "pegawaiselectdialog.h"

PegawaiUpdateDialog::PegawaiUpdateDialog(QWidget *parent) :
    QDialog(parent, Qt::WindowTitleHint | Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint)
{
    Engine *engine = Engine::instance();

    QLabel *nipLabel = new QLabel(tr("NIP"));
    QLabel *nip2Label = new QLabel(tr("NIP Baru"));
    QLabel *namaLabel = new QLabel(tr("Nama Lengkap"));
    QLabel *seksiLabel = new QLabel(tr("Seksi"));
    QLabel *pangkatLabel = new QLabel(tr("Pangkat"));
    QLabel *jabatanLabel = new QLabel(tr("Jabatan"));
    QLabel *plhLabel = new QLabel(tr("Plh"));

    mNipEdit = new QLineEdit;
    mNip2Edit = new QLineEdit;
    mNamaEdit = new QLineEdit;
    mSeksiCombo = new QComboBox;
    mPangkatCombo = new QComboBox;
    mJabatanCombo = new QComboBox;
    mPlhEdit = new QLineEdit;

    mNipEdit->setInputMask("000000000;_");
    mNip2Edit->setInputMask("000000000000000000;_");

    mNipEdit->setMaximumWidth(80);
    mNip2Edit->setMaximumWidth(135);
    mNamaEdit->setMinimumWidth(200);
    mSeksiCombo->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    mPangkatCombo->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    mJabatanCombo->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    mPlhEdit->setMaximumWidth(80);
    mPlhEdit->setEnabled(false);

    for (int i=0; i<engine->common()->namaPangkatList().size(); i++)
        mPangkatCombo->addItem(QString("%1 / %2").arg(engine->common()->namaPangkat(i), engine->common()->namaGolongan(i)), i);

    for (int i=0; i<engine->common()->namaJabatanList().size(); i++)
        mJabatanCombo->addItem(engine->common()->namaJabatan(i), i);

    mJabatanCombo->setCurrentIndex(mJabatanCombo->findData(JabatanPelaksana));

    QToolButton *plhButton = new QToolButton;
    plhButton->setText("Pilih");
    plhButton->setAutoRaise(true);

    QToolButton *hapusPlhButton = new QToolButton;
    hapusPlhButton->setText("Hapus");
    hapusPlhButton->setAutoRaise(true);

    QHBoxLayout *plhLayout = new QHBoxLayout;
    plhLayout->addWidget(mPlhEdit);
    plhLayout->addWidget(plhButton);
    plhLayout->addWidget(hapusPlhButton);
    plhLayout->addStretch();

    QPushButton *okButton = new QPushButton(tr("&Ok"));
    QPushButton *cancelButton = new QPushButton(tr("&Batal"));

    connect(plhButton, SIGNAL(clicked()), SLOT(selectPlh()));
    connect(hapusPlhButton, SIGNAL(clicked()), SLOT(removePlh()));
    connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));

    QHBoxLayout *buttonBox = new QHBoxLayout;
    buttonBox->addStretch();
    buttonBox->addWidget(okButton);
    buttonBox->addWidget(cancelButton);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(nipLabel, 0, 0, 1, 1);
    layout->addWidget(nip2Label, 1, 0, 1, 1);
    layout->addWidget(namaLabel, 2, 0, 1, 1);
    layout->addWidget(pangkatLabel, 3, 0, 1, 1);
    layout->addWidget(seksiLabel, 4, 0, 1, 1);
    layout->addWidget(jabatanLabel, 5, 0, 1, 1);
    layout->addWidget(plhLabel, 6, 0, 1, 1);
    layout->addWidget(mNipEdit, 0, 1, 1, 1);
    layout->addWidget(mNip2Edit, 1, 1, 1, 1);
    layout->addWidget(mNamaEdit, 2, 1, 1, 1);
    layout->addWidget(mPangkatCombo, 3, 1, 1, 1);
    layout->addWidget(mSeksiCombo, 4, 1, 1, 1);
    layout->addWidget(mJabatanCombo, 5, 1, 1, 1);
    layout->addLayout(plhLayout, 6, 1, 1, 1);
    layout->addLayout(buttonBox, 7, 0, 1, 2);

    setLayout(layout);
}

void PegawaiUpdateDialog::setKantor(const QString kodeKantor)
{
    Engine *engine = Engine::instance();
    mKodeKantor = kodeKantor;

    mSeksiCombo->clear();
    mSeksiCombo->addItem("", 0);
    foreach (const Seksi &seksi, engine->data()->seksiList(kodeKantor))
        mSeksiCombo->addItem(seksi.nama, seksi.id);
}

void PegawaiUpdateDialog::setNipPegawai(const QString &kodeKantor, const QString &nip, int tahun)
{
    Engine *engine = Engine::instance();
    Pegawai temp = engine->data()->pegawai(kodeKantor, tahun, nip);

    mNipEdit->setText(temp.nip);
    mNip2Edit->setText(temp.nip2);
    mNamaEdit->setText(temp.nama);
    mSeksiCombo->setCurrentIndex(mSeksiCombo->findData(temp.seksi.type));
    mPangkatCombo->setCurrentIndex(mPangkatCombo->findData(temp.pangkat.type));
    mJabatanCombo->setCurrentIndex(mJabatanCombo->findData(temp.jabatan.type));
    mPlhEdit->setText(temp.plh);
}

void PegawaiUpdateDialog::selectPlh()
{
    PegawaiSelectDialog dialog(this);
    dialog.setOnlyShowKantor(mKodeKantor);
    if (dialog.exec() == QDialog::Rejected)
        return;

    mPlhEdit->setText(dialog.nip());
}

void PegawaiUpdateDialog::removePlh()
{
    mPlhEdit->setText("");
}

void PegawaiUpdateDialog::setMode(int mode)
{
    if ( mode == AddMode )
        mNipEdit->setEnabled(true);
    else
        mNipEdit->setEnabled(false);
}

QString PegawaiUpdateDialog::nip()
{
    return mNipEdit->text();
}

QString PegawaiUpdateDialog::nip2()
{
    return mNip2Edit->text();
}

int PegawaiUpdateDialog::seksi()
{
    return mSeksiCombo->currentData().toInt();
}

QString PegawaiUpdateDialog::nama()
{
    return mNamaEdit->text();
}

int PegawaiUpdateDialog::pangkat()
{
    return mPangkatCombo->itemData(mPangkatCombo->currentIndex()).toInt();
}

int PegawaiUpdateDialog::jabatan()
{
    return mJabatanCombo->itemData(mJabatanCombo->currentIndex()).toInt();
}

QString PegawaiUpdateDialog::plh()
{
    return mPlhEdit->text();
}
