#ifndef EXAMPLEADDON_H
#define EXAMPLEADDON_H

#include "interface.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QDebug>

class ExamplePage : public Page
{
public:
    ExamplePage(QWidget *parent = nullptr) : Page(parent) {
        QLabel *label = new QLabel("Ini adalah halaman contoh");
        label->setAlignment(Qt::AlignCenter);

        QVBoxLayout *layout = new QVBoxLayout;
        layout->addWidget(label);

        setLayout(layout);
    }

    ~ExamplePage() {
        qDebug() << "Page Closed!";
    }

    void setArgs(const QVariantMap &) {}
};

class ExamplePageAddOn : public AddOn
{
public:
    QString name() { return "example_page"; }
    QString group() { return "Example"; }
    QString title() { return "Example AddOn Page"; }
    AddOnTypes type() { return AddOnPage; }
    int loadFlags() { return AddOnLoadCreateMenu; }
    int permission() { return 0; }
    Object *newObject() { return nullptr; }
    Page *newPage() { return new ExamplePage; }
    Process *newProcess() { return nullptr; }
};

class ExampleProcess : public Process
{
public:
    void run() {
        QMessageBox::information(nullptr, "Example Process", "Ini adalah contoh process yang jalan otomasis setelah login");
    }
};

class ExampleProcessAddOn : public AddOn
{
public:
    QString name() { return "example_process"; }
    QString group() { return "Example"; }
    QString title() { return "Example AddOn Process"; }
    AddOnTypes type() { return AddOnProcess; }
    int loadFlags() { return AddOnLoadCreateMenu | AddOnLoadExecAfterLogin; }
    int permission() { return 0; }
    Object *newObject() { return nullptr; }
    Page *newPage() { return nullptr; }
    Process *newProcess() { return new ExampleProcess; }
};
#endif // EXAMPLEADDON_H
