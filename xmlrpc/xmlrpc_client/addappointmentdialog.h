#ifndef ADDAPPOINTMENTDIALOG_H
#define ADDAPPOINTMENTDIALOG_H

#include <QDialog>
#include <QMap>
#include <QPushButton>
#include <QDateTimeEdit>
#include <QDateTime>
#include <QRegExp>
#include <QVariant>

namespace Ui {
class AddAppointmentDialog;
}

class AddAppointmentDialog : public QDialog
{
	Q_OBJECT

public:
	explicit AddAppointmentDialog(QWidget *parent = 0);
	~AddAppointmentDialog();

signals:
    sendData(QVariantList);

private slots:
	void on_buttonBox_accepted();
    void receiveData(QVariantMap data);
	void checkFields();
	void clearFields();

private:
	Ui::AddAppointmentDialog *ui;
	QString userId;
};

#endif // ADDAPPOINTMENTDIALOG_H
