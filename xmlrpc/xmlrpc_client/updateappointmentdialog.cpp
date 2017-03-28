#include "updateappointmentdialog.h"
#include "ui_updateappointmentdialog.h"

UpdateAppointmentDialog::UpdateAppointmentDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::UpdateAppointmentDialog)
{
	ui->setupUi(this);
	ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
	ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());
	connect(ui->lineEditAppointmentPlace, SIGNAL(textChanged(QString)), this, SLOT(checkFields()));
	connect(ui->textEditAppointmentInfo, SIGNAL(textChanged()), this, SLOT(checkFields()));
    connect(this, SIGNAL(sendData(QVariantList)), this, SLOT(clearFields()));
	connect(this, SIGNAL(rejected()), this, SLOT(clearFields()));
}

UpdateAppointmentDialog::~UpdateAppointmentDialog()
{
	delete ui;
}

void UpdateAppointmentDialog::receiveData(QVariantMap data) {
    ui->labelInfoUpdated->setText("Редактирование информации о приглашении \"" + data["place"].toString() + "\"");
    ui->lineEditAppointmentPlace->setText(data["place"].toString());
    ui->textEditAppointmentInfo->setText(data["info"].toString());
    ui->dateTimeEdit->setDateTime(QDateTime::fromString(data["date"].toString(), "ddd dd.MM.yy H:mm"));
    ui->lineEditAppointmentPriority->setText(data["priority"].toString());
    appointmentId = data["id"].toString();
}

void UpdateAppointmentDialog::on_buttonBox_accepted()
{
    QVariantList data = {
        appointmentId,
        ui->lineEditAppointmentPlace->text(),
        ui->textEditAppointmentInfo->toPlainText(),
        ui->dateTimeEdit->dateTime().toString("ddd dd.MM.yy H:mm"),
        ui->lineEditAppointmentPriority->text()
	};
	emit(sendData(data));
}

void UpdateAppointmentDialog::checkFields()
{
	QRegExp r("\\w{3,30}");
	if (!r.exactMatch(ui->lineEditAppointmentPlace->text())) {
		ui->lineEditAppointmentPlace->setStyleSheet("border: 1px solid red");
		ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
		return;
	}
	else
		ui->lineEditAppointmentPlace->setStyleSheet("border: 1px solid grey");
	if (ui->lineEditAppointmentPlace->text() != "" && ui->textEditAppointmentInfo->toPlainText() != "")
		ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
	else
		ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
}

void UpdateAppointmentDialog::clearFields()
{
	ui->lineEditAppointmentPlace->clear();
	ui->textEditAppointmentInfo->clear();
	ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());
	ui->lineEditAppointmentPriority->clear();
	ui->labelInfoUpdated->clear();
	ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
	ui->lineEditAppointmentPlace->setStyleSheet("border: 1px solid grey");
}
