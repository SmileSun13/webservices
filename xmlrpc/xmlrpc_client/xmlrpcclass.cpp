#include "xmlrpcclass.h"

XmlRPCClass::XmlRPCClass(QObject *parent) : QObject(parent)
{

}

void XmlRPCClass::responseMethod(QVariant &reply)
{

    QVariantList lst = reply.toList();
    QString methodName = lst.front().toString();
    QVector<QMap<QString, QString>> table;
    if (methodName == "get_users" || methodName == "get_user_appointments") {
        QVariantList tableTemp = lst.back().toList();
        for (int i = 0; i < tableTemp.size(); ++i) {
            auto j = tableTemp.at(i).toMap();
            QMap<QString, QString> temp;
            for(QVariantMap::const_iterator iter3 = j.begin(); iter3 != j.end(); ++iter3)
                temp[iter3.key()] = iter3.value().toString();
            table.append(temp);
        }
    }
    emit(dataReadyRead(methodName, lst.at(1).toString(), table));
}

void XmlRPCClass::faultResponse(int error, const QString &message)
{
    qDebug() << "An Error occoured, Code: " << error << " Message: " << message;
}

void XmlRPCClass::makeRequest(QString methodName, QVariantList data)
{
    client->call(methodName, data, this, SLOT(responseMethod(QVariant&)), this, SLOT(faultResponse(int, const QString &)));
}
