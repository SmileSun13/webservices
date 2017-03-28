#include "updateuserdialog.h"
#include "ui_updateuserdialog.h"

UpdateUserDialog::UpdateUserDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::UpdateUserDialog)
{
	ui->setupUi(this);
	ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
	connect(ui->lineEditUserName, SIGNAL(textChanged(QString)), this, SLOT(checkFields()));
	connect(ui->textEditUserInfo, SIGNAL(textChanged()), this, SLOT(checkFields()));
    connect(this, SIGNAL(sendData(QVariantList)), this, SLOT(clearFields()));
	connect(this, SIGNAL(rejected()), this, SLOT(clearFields()));
}

UpdateUserDialog::~UpdateUserDialog()
{
	delete ui;
}

void UpdateUserDialog::receiveData(QVariantMap data) {
    ui->labelUserChosen->setText("Редактирование информации о пользователе " + data["name"].toString());
    ui->lineEditUserName->setText(data["name"].toString());
    ui->textEditUserInfo->setText(data["info"].toString());
	ui->spinBoxFavNum->setValue(data["fav_num"].toInt());
    ui->comboBoxGender->setCurrentText(data["gender"].toString());
    userId = data["id"].toString();
}

void UpdateUserDialog::on_buttonBox_accepted()
{
    QVariantList data = {
        userId,
        ui->lineEditUserName->text(),
        ui->textEditUserInfo->toPlainText(),
        ui->spinBoxFavNum->text(),
        ui->comboBoxGender->currentText()
	};
    emit(sendData(data));
}

void UpdateUserDialog::checkFields()
{
	QRegExp r("\\w{3,30}");
	if (!r.exactMatch(ui->lineEditUserName->text())) {
		ui->lineEditUserName->setStyleSheet("border: 1px solid red");
		ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
		return;
	}
	else
		ui->lineEditUserName->setStyleSheet("border: 1px solid grey");
	if (ui->lineEditUserName->text() != "" && ui->textEditUserInfo->toPlainText() != "")
		ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
	else
		ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
}

void UpdateUserDialog::clearFields()
{
	ui->lineEditUserName->clear();
	ui->textEditUserInfo->clear();
	ui->spinBoxFavNum->clear();
	ui->comboBoxGender->setCurrentIndex(0);
	ui->labelUserChosen->clear();
	ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
	ui->lineEditUserName->setStyleSheet("border: 1px solid grey");
}
