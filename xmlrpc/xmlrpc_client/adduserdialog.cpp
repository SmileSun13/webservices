#include "adduserdialog.h"
#include "ui_adduserdialog.h"

AddUserDialog::AddUserDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::AddUserDialog)
{
	ui->setupUi(this);
	ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
	connect(ui->lineEditUserName, SIGNAL(textChanged(QString)), this, SLOT(checkFields()));
	connect(ui->textEditUserInfo, SIGNAL(textChanged()), this, SLOT(checkFields()));
    connect(this, SIGNAL(sendData(QVariantList)), this, SLOT(clearFields()));
	connect(this, SIGNAL(rejected()), this, SLOT(clearFields()));
}

AddUserDialog::~AddUserDialog()
{
	delete ui;
}

void AddUserDialog::checkFields()
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

void AddUserDialog::on_buttonBox_accepted()
{
    QVariantList data = {
        ui->lineEditUserName->text(),
        ui->textEditUserInfo->toPlainText(),
        ui->spinBoxFavNum->text(),
        ui->comboBoxGender->currentText()
	};
    emit(sendData(data));
}

void AddUserDialog::clearFields()
{
	ui->lineEditUserName->clear();
	ui->textEditUserInfo->clear();
	ui->spinBoxFavNum->setValue(0);
	ui->comboBoxGender->setCurrentIndex(0);
	ui->lineEditUserName->setStyleSheet("border: 1px solid grey");
	ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
}
