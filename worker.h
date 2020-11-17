#ifndef WORKER_H
#define WORKER_H
#include <QObject>
#include <QMutex>
#include <QJsonObject>

class Worker: public QObject
{
        Q_OBJECT
    public:
        Worker(QObject* parent = nullptr);
        ~Worker();
        void addTask(QJsonObject taskInfo);
        QJsonObject popTask();
        void stop();
    signals:
        void taskFinished(QJsonObject response);
    public slots:
        void doTask();
    private:
        QList<QJsonObject> m_taskList;
        QMutex m_mutex;
        bool m_bStart = true;
};

#endif // WORKER_H
