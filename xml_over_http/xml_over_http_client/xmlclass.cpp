#include "xmlclass.h"

XmlClass::XmlClass(QObject *parent) : QObject(parent) {}

void XmlClass::readXml(QByteArray xml, QMap<QString, QString> &tags, QVector<QMap<QString, QString> > &table_data)
{
	reader.clear();
	reader.addData(xml);
	while(!reader.atEnd()) {
		reader.readNext();
		if (reader.isStartElement() && reader.name() != "reply") {
			if (reader.name() != "table") {
				tags[reader.name().toString()] = reader.readElementText();
			}
			else {
				reader.readNext();
				while (!reader.atEnd()) {
					QMap<QString, QString> temp;
					reader.readNext();
					while (reader.name() != "row" && !reader.atEnd()) {
						if (reader.isStartElement()) {
							temp[reader.name().toString()] = reader.readElementText();
						}
						reader.readNext();
					}
					if (!temp.isEmpty())
						table_data.append(temp);
				}
			}
		}
	}
}

QByteArray XmlClass::writeXml(QMap<QString, QString> data)
{
	QByteArray byte_xml;
	QBuffer xml(&byte_xml);
	xml.open(QIODevice::WriteOnly);
	writer.setDevice(&xml);
	writer.writeStartDocument();
	writer.writeStartElement("request");
	QMap<QString, QString>::const_iterator i = data.constBegin();
	while (i != data.constEnd()) {
		writer.writeTextElement(i.key(), i.value());
		++i;
	}
	writer.writeEndElement();
	writer.writeEndDocument();
	xml.close();
	return byte_xml;
}
