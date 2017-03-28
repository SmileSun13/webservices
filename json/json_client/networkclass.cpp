#include "networkclass.h"
#include <QNetworkRequest>

NetworkClass::NetworkClass(QObject *parent) : QObject(parent)
{
	connect(qnam, SIGNAL(finished(QNetworkReply*)), this, SLOT(readRead(QNetworkReply*)));
}

void NetworkClass::makeRequest(QByteArray request)
{
	QUrl url("http://localhost:5000/");
	QNetworkRequest req(url);
	req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
	qnam->post(req, request);
}

void NetworkClass::readRead(QNetworkReply *reply)
{
	QByteArray myData;
	myData = reply->readAll();
	emit(dataReadyRead(myData));
}
