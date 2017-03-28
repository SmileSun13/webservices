#include "addappointmentdialog.h"
#include "ui_addappointmentdialog.h"

AddAppointmentDialog::AddAppointmentDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::AddAppointmentDialog)
{
	ui->setupUi(this);
	ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
	ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());
	connect(ui->lineEditAppointmentPlace, SIGNAL(textChanged(QString)), this, SLOT(checkFields()));
	connect(ui->textEditAppointmentInfo, SIGNAL(textChanged()), this, SLOT(checkFields()));
    connect(this, SIGNAL(sendData(QVariantList)), this, SLOT(clearFields()));
	connect(this, SIGNAL(rejected()), this, SLOT(clearFields()));
}

AddAppointmentDialog::~AddAppointmentDialog()
{
	delete ui;
}

void AddAppointmentDialog::receiveData(QVariantMap data) {
    ui->labelUserChosen->setText("Добавить приглашение пользователю " + data["name"].toString());
    userId = data["id"].toString();
}

void AddAppointmentDialog::on_buttonBox_accepted()
{
    QVariantList data = {
        userId,
        ui->lineEditAppointmentPlace->text(),
        ui->textEditAppointmentInfo->toPlainText(),
        ui->dateTimeEdit->dateTime().toString("ddd dd.MM.yy H:mm"),
        ui->lineEditAppointmentPriority->text()
	};
	emit(sendData(data));
}

void AddAppointmentDialog::checkFields()
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

void AddAppointmentDialog::clearFields()
{
	ui->lineEditAppointmentPlace->clear();
	ui->textEditAppointmentInfo->clear();
	ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());
	ui->lineEditAppointmentPriority->clear();
	ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
	ui->lineEditAppointmentPlace->setStyleSheet("border: 1px solid grey");
}
