#ifndef XMLCLASS_H
#define XMLCLASS_H

#include <QObject>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QMap>
#include <QVector>
#include <QBuffer>

class XmlClass : public QObject
{
	Q_OBJECT
public:
	explicit XmlClass(QObject *parent = 0);
	void readXml(QByteArray xml, QMap<QString, QString>&tags, QVector<QMap<QString, QString>>&table_data);
	QByteArray writeXml(QMap<QString, QString> data);

signals:

public slots:

private:
	QXmlStreamReader reader;
	QXmlStreamWriter writer;
};

#endif // XMLCLASS_H
