#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStackedLayout>
#include <QWebChannel>
#include <QMessageBox>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QThread>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    qputenv("QTWEBENGINE_REMOTE_DEBUGGING", "7777");
    ui->setupUi(this);
    QStackedLayout* layout = new QStackedLayout(ui->webView);
    ui->webView->setLayout(layout);
    web = new QWebEngineView(this);
    layout->addWidget(web);

    initWorkerThread();
    initWebEngine();
    initDevToolWindow();
    initSerialPort();
}

MainWindow::~MainWindow()
{ 
    if(threadWorker->isRunning())
    {
        threadWorker->quit();
    }
    threadWorker->wait();

    serial->close();
    delete ui;
    delete web;
    delete devWindow;
    delete apiRouter;
    delete channel;
    delete serial;
}

void MainWindow::initWebEngine()
{
    channel = new QWebChannel(this);
    connect(apiRouter, &ApiRouter::receiveMsgFromClient, this, &MainWindow::OnReceiveMessageFromJS);
    channel->registerObject(QStringLiteral("context"), apiRouter);
    web->page()->setWebChannel(channel);
    web->load(QUrl("qrc:///dist/index.html"));
}

void MainWindow::initWorkerThread()
{
    threadWorker = new QThread();
    worker = new Worker();
    worker->moveToThread(threadWorker);
    apiRouter = new ApiRouter(worker);
    connect(threadWorker, &QThread::finished, threadWorker, &QObject::deleteLater);
    connect(threadWorker, &QThread::finished, worker, &QObject::deleteLater);
    connect(threadWorker, &QThread::started, worker, &Worker::doTask);
    threadWorker->start();
}

void MainWindow::initDevToolWindow()
{
    web->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(web, &QWidget::customContextMenuRequested, this, [this]() {
        QMenu* menu = new QMenu(this);
        QAction* devTool = menu->addAction("Dev Tools");
        connect(devTool, &QAction::triggered, this, [this](){
            if (devWindow == NULL) {
                devWindow = new DevWindow();
                devWindow->show();
            }
            else {
                devWindow->show();
            }
        });

        QAction* pageRefresh = menu->addAction("Refresh");
        connect(pageRefresh, &QAction::triggered, this, [this](){
            web->page()->triggerAction(QWebEnginePage::Reload);
        });

        QAction* routeBack = menu->addAction("Back");
        connect(routeBack, &QAction::triggered, this, [this](){
            web->page()->triggerAction(QWebEnginePage::Back);
        });

        QAction* routeForward = menu->addAction("Previous");
        connect(routeForward, &QAction::triggered, this, [this](){
            web->page()->triggerAction(QWebEnginePage::Forward);
        });

        menu->exec(QCursor::pos());
    });
}

void MainWindow::initSerialPort()
{
    //创建串口对象
    serial = new QSerialPort(this);
    //设置串口名
    serial->setPortName("COM1");
    //设置波特率
    serial->setBaudRate(QSerialPort::Baud115200);
    //设置数据位数
    serial->setDataBits(QSerialPort::Data8);
    //设置奇偶校验
    serial->setParity(QSerialPort::NoParity);
    //设置停止位
    serial->setStopBits(QSerialPort::OneStop);
    //设置流控制
    serial->setFlowControl(QSerialPort::NoFlowControl);
    //打开串口
    serial->open(QIODevice::ReadWrite);

    connect(serial, &QSerialPort::readyRead, this, &MainWindow::OnReceiveMessageFromSerial);
}

void MainWindow::resizeEvent(QResizeEvent *)
{
    ui->mainContent->resize(this->width() - 20, this->height() - 20);
}

void MainWindow::on_sendBtn_clicked()
{
    QString str = ui->sendEdit->toPlainText();
    if(!str.isEmpty()) {
        apiRouter->sendMsgToClient(str);
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
    history += "[" + time + "]" + "JS: " + strParameter + "\n";
    ui->msgBrowser->setText(history);

    serial->write(strParameter.toUtf8());
}

void MainWindow::OnReceiveMessageFromSerial() {
    QByteArray buffer = serial->readAll();
    QString history = ui->msgBrowser->toPlainText();
    QString time = QDate::currentDate().toString("yyyy-MM-dd ") + QTime::currentTime().toString("HH:mm:ss");
    history += "[" + time + "]" + "UART: " + QString(buffer) + "\n";
    ui->msgBrowser->setText(history);
}
