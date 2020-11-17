#include "worker.h"
#include "apirouter.h"
#include <QThread>

Worker::Worker(QObject* parent): QObject(parent)
{

}

Worker::~Worker()
{

}

void Worker::doTask() {
    while (m_bStart) {
        QJsonObject taskInfo = popTask();

        if(taskInfo.isEmpty()) {
            continue;
        }

        if(taskInfo["action"].toString() == "send-msg")
        {
            QJsonObject response = ApiRouter::formatResponse(taskInfo, "Get Msg");

            emit taskFinished(response);
        }
        else if(taskInfo["action"].toString() == "send-msg-sync")
        {
            QThread::sleep(5);
            QJsonObject response = ApiRouter::formatResponse(taskInfo, "Sync Msg After 5s");

            emit taskFinished(response);
        }
    }
}

void Worker::stop() {
    m_bStart = false;
}

void Worker::addTask(QJsonObject taskInfo) {
    m_mutex.lock();
    m_taskList.push_front(taskInfo);
    m_mutex.unlock();
}

QJsonObject Worker::popTask()
{
    m_mutex.lock();
    QJsonObject taskInfo;
    if(m_taskList.size() == 0){
        taskInfo.empty();
    }
    else{
        taskInfo = m_taskList.takeFirst();
    }
    m_mutex.unlock();
    return taskInfo;
}


