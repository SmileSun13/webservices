#ifndef XMLRPCCLASS_H
#define XMLRPCCLASS_H

#include <QObject>
#include <QVariant>
#include "maiaXmlRpcClient.h"

class XmlRPCClass : public QObject
{
    Q_OBJECT
public:
    explicit XmlRPCClass(QObject *parent = 0);
    void makeRequest(QString methodName, QVariantList data);


signals:
    void dataReadyRead(QString, QString, QVector<QMap<QString, QString>>);
public slots:
    void responseMethod(QVariant &reply);
    void faultResponse(int error, const QString &message);
private:
    MaiaXmlRpcClient *client = new MaiaXmlRpcClient(QUrl("http://localhost:8000/RPC2"), this);

};

#endif // XMLRPCCLASS_H
