#ifndef NETWORKCLASS_H
#define NETWORKCLASS_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class NetworkClass : public QObject
{
	Q_OBJECT
public:
	explicit NetworkClass(QObject *parent = 0);
	void makeRequest(QByteArray request);
signals:
	void dataReadyRead(QByteArray);
public slots:
	void readRead(QNetworkReply *reply);
private:
	QNetworkAccessManager *qnam = new QNetworkAccessManager(this);
};

#endif // NETWORKCLASS_H
