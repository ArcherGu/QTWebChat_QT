#ifndef WEBBRIDGE_H
#define WEBBRIDGE_H

#include <QObject>


class WebBridge: public QObject
{
    Q_OBJECT
    public:
        WebBridge();
//        Q_INVOKABLE void JSSendMessage(QString strParameter);

    public slots:
        void invokedByClient();
        void QTSendMessage(QString strParameter);
        QString JSSendMessage(QString strParameter);

    signals:
        void SigSendMessageToJS(QString strParameter);
        void SigReceviceMessageFromJS(QString strParameter);
};


#endif // WEBBRIDGE_H
