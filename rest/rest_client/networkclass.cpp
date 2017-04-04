#include "networkclass.h"
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>

NetworkClass::NetworkClass(QObject *parent) : QObject(parent)
{
	connect(qnam, SIGNAL(finished(QNetworkReply*)), this, SLOT(readRead(QNetworkReply*)));
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
}

void NetworkClass::makeRequest(QString methodName, QByteArray request)
{
    if (methodName == "add_user") {
        QUrl url(urlStr+"/user");
        req.setUrl(url);
        qnam->post(req, request);
    }
}

void NetworkClass::makeRequest(QString methodName)
{
    if (methodName == "get_users") {
        QUrl url(urlStr+"/user");
        req.setUrl(url);
        qnam->get(req);
    }
}

void NetworkClass::makeRequest(QString methodName, QString id)
{
    QJsonObject json;
    QByteArray request = QJsonDocument(json).toJson();
    if (methodName == "delete_user") {
        QUrl url(urlStr+"/user/"+id);
        req.setUrl(url);
        qnam->post(req, request);
    }
    else if (methodName == "delete_appointment") {
        QUrl url(urlStr+"/appointment/"+id);
        req.setUrl(url);
        qnam->post(req, request);
    }
    else if (methodName == "get_user_appointments") {
        QUrl url(urlStr+"/user/"+id+"/appointment");
        req.setUrl(url);
        qnam->get(req);
    }
}

void NetworkClass::makeRequest(QString methodName, QString id, QByteArray request)
{
    if (methodName == "update_user") {
        QUrl url(urlStr+"/user/"+id);
        req.setUrl(url);
        qnam->post(req, request);
    }
    else if (methodName == "update_appointment") {
        QUrl url(urlStr+"/appointment/"+id);
        req.setUrl(url);
        qnam->post(req, request);
    }
    else if (methodName == "add_appointment") {
        QUrl url(urlStr+"/user/"+id+"/appointment");
        req.setUrl(url);
        qnam->post(req, request);
    }
}

void NetworkClass::readRead(QNetworkReply *reply)
{
	QByteArray myData;
	myData = reply->readAll();
	emit(dataReadyRead(myData));
}
