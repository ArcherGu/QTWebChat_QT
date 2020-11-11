#include "webbridge.h"
#include <QMessageBox>
#include "iostream"

WebBridge::WebBridge()
{

}

void WebBridge::invoked_by_client()
{
    QMessageBox::information(NULL,"invokedByClient","I'm called by client JS!");
}

QString WebBridge::send_msg(QString strParameter)
{
    emit SigReceviceMessageFromJS(strParameter);

    return QString("收到消息了！");
}

void WebBridge::sendMsgToJs(QString strParameter)
{
    emit SigReceviceMessageFromQT(strParameter);
}
