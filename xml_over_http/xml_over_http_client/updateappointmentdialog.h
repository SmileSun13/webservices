#ifndef UPDATEAPPOINTMENTDIALOG_H
#define UPDATEAPPOINTMENTDIALOG_H

#include <QDialog>
#include <QMap>
#include <QPushButton>
#include <QDateTimeEdit>
#include <QDateTime>
#include <QRegExp>

namespace Ui {
class UpdateAppointmentDialog;
}

class UpdateAppointmentDialog : public QDialog
{
	Q_OBJECT

public:
	explicit UpdateAppointmentDialog(QWidget *parent = 0);
	~UpdateAppointmentDialog();

signals:
	sendData(QMap<QString, QString>);

private slots:
	void on_buttonBox_accepted();
	void receiveData(QMap<QString, QString> data);
	void checkFields();
	void clearFields();

private:
	Ui::UpdateAppointmentDialog *ui;
	QString appointmentId;
};

#endif // UPDATEAPPOINTMENTDIALOG_H
