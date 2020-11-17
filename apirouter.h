#ifndef APIROUTER_H
#define APIROUTER_H
#include <QObject>
#include "worker.h"

class ApiRouter: public QObject
{
    Q_OBJECT
    public:
        ApiRouter(Worker* worker);
        void sendMsgToClient(QString strParameter);
        static QJsonObject formatResponse(QJsonObject taskInfo, QString msg);
    public slots:
        void requestFromClient(QString strParameter);
    signals:
        void responseFromServer(QString strParameter);
        void receiveMsgFromClient(QString strParameter);
        void receiveMsgFromServer(QString strParameter);

    private slots:
        void onTaskFinished(QJsonObject response);
    private:
        Worker* m_worker = nullptr;
};

#endif // APIROUTER_H
