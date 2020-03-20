#include "devwindow.h"
#include "ui_devwindow.h"
#include <QStackedLayout>

DevWindow::DevWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DevWindow)
{
    ui->setupUi(this);
    devView = new QWebEngineView(this);

    connect(ui->btnClose, &QPushButton::clicked, this, [this](){
        hide();
    });
    connect(ui->btnBack, &QPushButton::clicked, this, [this](){
        devView->page()->triggerAction(QWebEnginePage::Back);
    });


    QStackedLayout* layout = new QStackedLayout(ui->frame);
    ui->frame->setLayout(layout);
    layout->addWidget(devView);
    devView->load(QUrl("http://localhost:7777"));
    QDialog::show();
}

DevWindow::~DevWindow()
{
    delete ui;
}

void DevWindow::show()
{
    devView->reload();
    QDialog::show();
}

void DevWindow::resizeEvent(QResizeEvent *)
{
    ui->widget->resize(this->size());
}
