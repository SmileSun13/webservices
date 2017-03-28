#include "jsonclass.h"

JsonClass::JsonClass(QObject *parent) : QObject(parent)
{

}

void JsonClass::readJson(QByteArray data, QMap<QString, QString>&tags, QVector<QMap<QString, QString>>&table)
{
	QVariantMap varMap = QJsonDocument::fromJson(data).object().toVariantMap();
	for(QVariantMap::const_iterator iter = varMap.begin(); iter != varMap.end(); ++iter) {
		if (iter.key() != "table")
			tags[iter.key()] = iter.value().toString();
		else {
			auto tableTemp = iter.value().toList();
			for (int i = 0; i < tableTemp.size(); ++i) {
				auto j = tableTemp.at(i).toMap();
				QMap<QString, QString> temp;
				for(QVariantMap::const_iterator iter3 = j.begin(); iter3 != j.end(); ++iter3)
					temp[iter3.key()] = iter3.value().toString();
				table.append(temp);
			}
		}
	}
}

QByteArray JsonClass::writeJson(QMap<QString, QString> data)
{
	QJsonObject json;
	QMap<QString, QString>::const_iterator i = data.constBegin();
	while (i != data.constEnd()) {
		json.insert(i.key(), i.value());
		++i;
	}
	return QJsonDocument(json).toJson();
}
