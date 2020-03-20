#include "webbridge.h"
#include <QMessageBox>
#include "iostream"

WebBridge::WebBridge()
{

}

void WebBridge::invokedByClient()
{
    QMessageBox::information(NULL,"invokedByClient","I'm called by client JS!");
}

QString WebBridge::JSSendMessage(QString strParameter)
{
    emit SigReceviceMessageFromJS(strParameter);

    return QString("收到消息了！");
}

void WebBridge::QTSendMessage(QString strParameter)
{
    emit SigSendMessageToJS(strParameter);
}
