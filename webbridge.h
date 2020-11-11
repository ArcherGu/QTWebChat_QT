#ifndef WEBBRIDGE_H
#define WEBBRIDGE_H

#include <QObject>


class WebBridge: public QObject
{
    Q_OBJECT
    public:
        WebBridge();
        void sendMsgToJs(QString strParameter);

    public slots:
        void invoked_by_client();
        QString send_msg(QString strParameter);

    signals:
        void SigReceviceMessageFromQT(QString strParameter);
        void SigReceviceMessageFromJS(QString strParameter);
};


#endif // WEBBRIDGE_H
