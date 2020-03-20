#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWebEngineView>
#include "devwindow.h"
#include "webbridge.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
protected:
    void resizeEvent(QResizeEvent *);
    void customContextMenu();
private slots:
    void on_sendBtn_clicked();
    void OnReceiveMessageFromJS(QString strParameter);

private:
    Ui::MainWindow *ui;
    QWebEngineView *web;
    DevWindow *devWindow = NULL;
    WebBridge *webBridge;
};
#endif // MAINWINDOW_H
