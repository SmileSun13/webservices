#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
	addUser = new AddUserDialog();
	updateUser = new UpdateUserDialog();
	addAppointment = new AddAppointmentDialog();
	updateAppointment = new UpdateAppointmentDialog();
	setupConnections();
	ui->tableUsers->setColumnWidth(0, 30);
	ui->tableAppointments->setColumnWidth(0, 30);
    updateTable("get_users");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupConnections()
{
	connect(ui->buttonAddUser, SIGNAL(clicked()), addUser, SLOT(show()));
	connect(ui->buttonUpdateUser, SIGNAL(clicked()), this, SLOT(prepareData()));
	connect(ui->buttonDeleteUser, SIGNAL(clicked()), this, SLOT(deleteRow()));
	connect(ui->buttonDeleteAppointment, SIGNAL(clicked()), this, SLOT(deleteRow()));
	connect(ui->buttonAddAppointment, SIGNAL(clicked()), this, SLOT(prepareData()));
	connect(ui->buttonUpdateAppointment, SIGNAL(clicked()), this, SLOT(prepareData()));
	connect(&net, SIGNAL(dataReadyRead(QByteArray)), this, SLOT(dataSlot(QByteArray)));
	connect(ui->tableUsers, SIGNAL(itemSelectionChanged()), this, SLOT(checkSelection()));
	connect(ui->tableAppointments, SIGNAL(itemSelectionChanged()), this, SLOT(checkSelection()));
	connect(addUser, SIGNAL(sendData(QMap<QString, QString>)), this, SLOT(receiveData(QMap<QString, QString>)));
	connect(updateUser, SIGNAL(sendData(QMap<QString, QString>)), this, SLOT(receiveData(QMap<QString, QString>)));
	connect(this, SIGNAL(sendData(QMap<QString, QString>)), updateUser, SLOT(receiveData(QMap<QString, QString>)));
	connect(addAppointment, SIGNAL(sendData(QMap<QString, QString>)), this, SLOT(receiveData(QMap<QString, QString>)));
	connect(this, SIGNAL(sendData(QMap<QString, QString>)), addAppointment, SLOT(receiveData(QMap<QString, QString>)));
	connect(updateAppointment, SIGNAL(sendData(QMap<QString, QString>)), this, SLOT(receiveData(QMap<QString, QString>)));
	connect(this, SIGNAL(sendData(QMap<QString, QString>)), updateAppointment, SLOT(receiveData(QMap<QString, QString>)));
}

void MainWindow::showMessage(QMap<QString, QString> tags)
{
	if (tags["message"] == "good") {
		QString goodMessage;
        if (tags["action"] == "add_user")
			goodMessage = "Пользователь успешно добавлен.";
        else if (tags["action"] == "update_user")
			goodMessage = "Данные пользователя успешно обновлены.";
        else if (tags["action"] == "add_appointment")
            goodMessage = "Встреча успешно добавлена.";
        else if (tags["action"] == "update_appointment")
            goodMessage = "Данные о встрече успешно обновлены.";
        else if (tags["action"] == "delete_appointment")
            goodMessage = "Встреча успешно удалена.";
        else if (tags["action"] == "delete_user")
			goodMessage = "Пользователь успешно удалён.";
		QMessageBox* goodBox =
							new QMessageBox("Операция совершена успешно",
							goodMessage,
							QMessageBox::Information,
							QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
		goodBox->exec();
		delete goodBox;
        if (tags["action"] == "delete_user" || tags["action"] == "add_user" || tags["action"] == "update_user") {
            updateTable("get_users");
		}
        else if (tags["action"] == "add_appointment" || tags["action"] == "update_appointment" || tags["action"] == "delete_appointment") {
            updateTable("get_user_appointments", ui->tableUsers->selectedItems()[0]->text());
		}
	}
	else {
		QString errorMessage;
        if (tags["action"] == "add_user" || tags["action"] == "update_user") {
			errorMessage = "Невозможно присвоить пользователю данное имя, так как оно уже занято.";
		}
		QMessageBox* errorBox =
							new QMessageBox("Ошибка",
							errorMessage,
							QMessageBox::Critical,
							QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
		errorBox->exec();
		delete errorBox;
	}
}

void MainWindow::receiveData(QMap<QString, QString> data) {
    QString name = sender()->objectName();
    if (name == "AddUserDialog"){
        net.makeRequest("add_user", json.writeJson(data));
    }
    else if (name == "UpdateUserDialog"){
        QString id = data["id"];
        data.remove("id");
        net.makeRequest("update_user", id, json.writeJson(data));
    }
    else if (name == "AddAppointmentDialog"){
        QString id = data["id"];
        data.remove("id");
        net.makeRequest("add_appointment", id, json.writeJson(data));
    }
    else if (name == "UpdateAppointmentDialog"){
        QString id = data["id"];
        data.remove("id");
        net.makeRequest("update_appointment", id, json.writeJson(data));
    }
}

void MainWindow::prepareData() {
	auto j = sender()->objectName();
	if (sender()->objectName() == "buttonUpdateUser") {
		QMap<QString, QString> data = {
			{"id", ui->tableUsers->selectedItems()[0]->text()},
			{"name", ui->tableUsers->selectedItems()[1]->text()},
			{"info", ui->tableUsers->selectedItems()[2]->text()},
			{"fav_num", ui->tableUsers->selectedItems()[3]->text()},
			{"gender", ui->tableUsers->selectedItems()[4]->text()}
		};
		updateUser->show();
		emit(sendData(data));
	}
	else if (sender()->objectName() == "buttonUpdateAppointment") {
		QMap<QString, QString> data = {
			{"id", ui->tableAppointments->selectedItems()[0]->text()},
			{"place", ui->tableAppointments->selectedItems()[1]->text()},
			{"info", ui->tableAppointments->selectedItems()[2]->text()},
			{"date", ui->tableAppointments->selectedItems()[3]->text()},
			{"priority", ui->tableAppointments->selectedItems()[4]->text()}
		};
		updateAppointment->show();
		emit(sendData(data));
	}
	else if (sender()->objectName() == "buttonAddAppointment") {
		QMap<QString, QString> data = {
			{"id", ui->tableUsers->selectedItems()[0]->text()},
			{"name", ui->tableUsers->selectedItems()[1]->text()}
		};
		addAppointment->show();
		emit(sendData(data));
	}
}

void MainWindow::updateTable(QString actionType, QString userId) {
    if (actionType == "get_user_appointments")
        net.makeRequest("get_user_appointments", userId);
    else
        net.makeRequest("get_users");
}

void MainWindow::checkSelection() {
	if (sender()->objectName() == "tableUsers") {
		QList<QTableWidgetSelectionRange> it = ui->tableUsers->selectedRanges();
		if (it.count() != 1 || it.first().columnCount() != 5 || it.first().rowCount() != 1){
			ui->buttonUpdateUser->setEnabled(false);
			ui->buttonDeleteUser->setEnabled(false);
			ui->buttonAddAppointment->setEnabled(false);
			ui->tableAppointments->clearContents();
			ui->tableAppointments->setRowCount(0);
		}
		else if (it.count() == 1 && it.first().columnCount() == 5 && it.first().rowCount() == 1){
			ui->buttonUpdateUser->setEnabled(true);
            updateTable("get_user_appointments", ui->tableUsers->selectedItems()[0]->text());
		}
	}
	else {
		QList<QTableWidgetSelectionRange> it = ui->tableAppointments->selectedRanges();
		if (it.count() != 1 || it.first().columnCount() != 5 || it.first().rowCount() != 1){
			ui->buttonUpdateAppointment->setEnabled(false);
			ui->buttonDeleteAppointment->setEnabled(false);
		}
		else if (it.count() == 1 && it.first().columnCount() == 5 && it.first().rowCount() == 1){
			ui->buttonUpdateAppointment->setEnabled(true);
			ui->buttonDeleteAppointment->setEnabled(true);
		}
	}
}

void MainWindow::dataSlot(QByteArray data)
{
	QMap<QString, QString> tags;
	QVector<QMap<QString, QString>> tableData;
	json.readJson(data, tags, tableData);
    if (tags["action"] == "get_user_appointments" || tags["action"] == "get_users") {
		QTableWidget *table;
        if (tags["action"] == "get_users")
			table = ui->tableUsers;
        else if (tags["action"] == "get_user_appointments")
			table = ui->tableAppointments;
		table->clearContents();
		table->setRowCount(tableData.size());
		for (int i=0; i<tableData.size(); i++) {
			QMap<QString, QString>::const_iterator k = tableData[i].constBegin();
			while (k != tableData[i].constEnd()) {
				QTableWidgetItem *item = new QTableWidgetItem(k.value());
				if (k.key() == "id")
					table->setItem(i, 0, item);
				else if (k.key() == "name" || k.key() == "place")
					table->setItem(i, 1, item);
				else if (k.key() == "info")
					table->setItem(i, 2, item);
				else if (k.key() == "fav_num" || k.key() == "date")
					table->setItem(i, 3, item);
				else if (k.key() == "gender" || k.key() == "priority")
					table->setItem(i, 4, item);
				++k;
			}
		}
        if (tags["action"] == "get_user_appointments" && table->rowCount() > 0)
			ui->buttonDeleteUser->setEnabled(false);
        else if (tags["action"] == "get_user_appointments" && table->rowCount() == 0)
			ui->buttonDeleteUser->setEnabled(true);
        if (tags["action"] == "get_user_appointments")
			ui->buttonAddAppointment->setEnabled(true);
	}
	else
		showMessage(tags);
}

void MainWindow::deleteRow()
{
    if (sender()->objectName() == "buttonDeleteUser")
        net.makeRequest("delete_user", ui->tableUsers->selectedItems()[0]->text());
    else
        net.makeRequest("delete_appointment", ui->tableAppointments->selectedItems()[0]->text());
}
