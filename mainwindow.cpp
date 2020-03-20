#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStackedLayout>
#include <QWebChannel>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    qputenv("QTWEBENGINE_REMOTE_DEBUGGING", "7777");
    ui->setupUi(this);
    web = new QWebEngineView(this);
    QStackedLayout *layout = new QStackedLayout(ui->webView);
    ui->webView->setLayout(layout);
    layout->addWidget(web);

    customContextMenu();

    QWebChannel *channel = new QWebChannel(this);
    webBridge = new WebBridge();
    channel->registerObject(QStringLiteral("context"), webBridge);
    web->page()->setWebChannel(channel);
    connect(webBridge, &WebBridge::SigReceviceMessageFromJS, this, &MainWindow::OnReceiveMessageFromJS);
    web->load(QUrl("qrc:///dist/index.html"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::customContextMenu()
{
    web->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(web, &QWidget::customContextMenuRequested, this, [this]() {
        QMenu* menu = new QMenu(this);
        QAction* devTool = menu->addAction("开发者工具");
        connect(devTool, &QAction::triggered, this, [this](){
            if (devWindow == NULL) {
                devWindow = new DevWindow();
                devWindow->show();
            }
            else {
                devWindow->show();
            }
        });

        QAction* pageRefresh = menu->addAction("刷新");
        connect(pageRefresh, &QAction::triggered, this, [this](){
            web->page()->triggerAction(QWebEnginePage::Reload);
        });

        QAction* routeBack = menu->addAction("后退");
        connect(routeBack, &QAction::triggered, this, [this](){
            web->page()->triggerAction(QWebEnginePage::Back);
        });

        QAction* routeForward = menu->addAction("前进");
        connect(routeForward, &QAction::triggered, this, [this](){
            web->page()->triggerAction(QWebEnginePage::Forward);
        });

        menu->exec(QCursor::pos());
    });
}

void MainWindow::resizeEvent(QResizeEvent *)
{
    ui->mainContent->resize(this->width() - 20, this->height() - 20);
}

void MainWindow::on_sendBtn_clicked()
{
    QString str = ui->sendEdit->toPlainText();
    if(!str.isEmpty()) {
        webBridge->QTSendMessage(str);
        ui->sendEdit->setText("");
        QString history = ui->msgBrowser->toPlainText();
        QString time = QDate::currentDate().toString("yyyy-MM-dd ") + QTime::currentTime().toString("HH:mm:ss");
        history += "[" + time + "]" + "QT: " + str + "\n";
        ui->msgBrowser->setText(history);
    }
    else {
        QMessageBox::information(NULL,"警告","请输入内容后再发送！");
    }
}

void MainWindow::OnReceiveMessageFromJS(QString strParameter) {
    QString history = ui->msgBrowser->toPlainText();
    QString time = QDate::currentDate().toString("yyyy-MM-dd ") + QTime::currentTime().toString("HH:mm:ss");
    history += "[" + time + "]" + "QT: " + strParameter + "\n";
    ui->msgBrowser->setText(history);
}
