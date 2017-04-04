#ifndef JSONCLASS_H
#define JSONCLASS_H

#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMap>
#include <QVariantMap>
#include <QVector>
#include <QList>

class JsonClass : public QObject
{
	Q_OBJECT
public:
	explicit JsonClass(QObject *parent = 0);
	void readJson(QByteArray data, QMap<QString, QString> &tags, QVector<QMap<QString, QString> > &table);
	QByteArray writeJson(QMap<QString, QString> data);

signals:

public slots:
};

#endif // JSONCLASS_H
