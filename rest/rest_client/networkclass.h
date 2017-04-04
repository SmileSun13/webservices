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
    void makeRequest(QString methodName, QByteArray request);
    void makeRequest(QString methodName);
    void makeRequest(QString methodName, QString id);
    void makeRequest(QString methodName, QString id, QByteArray request);

signals:
	void dataReadyRead(QByteArray);
public slots:
	void readRead(QNetworkReply *reply);
private:
	QNetworkAccessManager *qnam = new QNetworkAccessManager(this);
    QString urlStr = "http://localhost:5000";
    QNetworkRequest req;
};

#endif // NETWORKCLASS_H
