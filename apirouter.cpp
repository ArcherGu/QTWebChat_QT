#include "apirouter.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QDebug>

ApiRouter::ApiRouter(Worker* worker)
{
    m_worker = worker;
    connect(m_worker, &Worker::taskFinished, this, &ApiRouter::onTaskFinished);
}

void ApiRouter::requestFromClient(QString strParameter) {
    QJsonParseError json_error;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(strParameter.toLocal8Bit().data(), &json_error);

    if(json_error.error != QJsonParseError::NoError)
    {
        qDebug() << "json parse error!";
        throw  QString("json parse error!");
    }

    QJsonObject jsonObj = jsonDoc.object();

    m_worker->addTask(jsonObj);
}

void ApiRouter::onTaskFinished(QJsonObject response)
{
    QString responseJson = QString(QJsonDocument(response).toJson(QJsonDocument::Compact));
    QString msg = response["msg"].toString();
    emit responseFromServer(responseJson);
    emit receiveMsgFromClient(msg);
}

void ApiRouter::sendMsgToClient(QString strParameter)
{
    emit receiveMsgFromServer(strParameter);
}

QJsonObject ApiRouter::formatResponse(QJsonObject taskInfo, QString msg)
{
    QJsonObject response;
    response.insert("id", taskInfo["id"]);
    response.insert("data", msg);
    response.insert("msg", taskInfo["data"]);
    return response;
}
