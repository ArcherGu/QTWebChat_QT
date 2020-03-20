#ifndef DEVWINDOW_H
#define DEVWINDOW_H

#include <QDialog>
#include <QWebEngineView>

namespace Ui {
    class DevWindow;
}

class DevWindow : public QDialog
{
    Q_OBJECT

public:
    explicit DevWindow(QWidget *parent = nullptr);
    ~DevWindow();
    void show();
protected:
    void resizeEvent(QResizeEvent *);
private:
    Ui::DevWindow *ui;
    QWebEngineView* devView;
};

#endif // DEVWINDOW_H
