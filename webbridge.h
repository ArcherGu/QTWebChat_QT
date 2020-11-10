#ifndef WEBBRIDGE_H
#define WEBBRIDGE_H

#include <QObject>


class WebBridge: public QObject
{
    Q_OBJECT
    public:
        WebBridge();
        void QTSendMessage(QString strParameter);

    public slots:
        void invokedByClient();
        QString JSSendMessage(QString strParameter);

    signals:
        void SigSendMessageToJS(QString strParameter);
        void SigReceviceMessageFromJS(QString strParameter);
};


#endif // WEBBRIDGE_H
