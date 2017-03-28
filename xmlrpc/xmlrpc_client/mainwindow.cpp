#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
	addUser = new AddUserDialog(this);
	updateUser = new UpdateUserDialog(this);
	addAppointment = new AddAppointmentDialog(this);
	updateAppointment = new UpdateAppointmentDialog(this);
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
    connect(&rpc, SIGNAL(dataReadyRead(QString, QString, QVector<QMap<QString, QString>>)), this, SLOT(dataSlot(QString, QString, QVector<QMap<QString, QString>>)));
	connect(ui->tableUsers, SIGNAL(itemSelectionChanged()), this, SLOT(checkSelection()));
	connect(ui->tableAppointments, SIGNAL(itemSelectionChanged()), this, SLOT(checkSelection()));
    connect(addUser, SIGNAL(sendData(QVariantList)), this, SLOT(receiveData(QVariantList)));
    connect(updateUser, SIGNAL(sendData(QVariantList)), this, SLOT(receiveData(QVariantList)));
    connect(this, SIGNAL(sendData(QVariantMap)), updateUser, SLOT(receiveData(QVariantMap)));
    connect(addAppointment, SIGNAL(sendData(QVariantList)), this, SLOT(receiveData(QVariantList)));
    connect(this, SIGNAL(sendData(QVariantMap)), addAppointment, SLOT(receiveData(QVariantMap)));
    connect(updateAppointment, SIGNAL(sendData(QVariantList)), this, SLOT(receiveData(QVariantList)));
    connect(this, SIGNAL(sendData(QVariantMap)), updateAppointment, SLOT(receiveData(QVariantMap)));
}

void MainWindow::showMessage(QString methodName, QString message)
{
    if (message == "good") {
		QString goodMessage;
        if (methodName == "add_user")
			goodMessage = "Пользователь успешно добавлен.";
        else if (methodName == "update_user")
			goodMessage = "Данные пользователя успешно обновлены.";
        else if (methodName == "add_appointment")
            goodMessage = "Встреча успешно добавлена.";
        else if (methodName == "update_appointment")
            goodMessage = "Данные о встрече успешно обновлены.";
        else if (methodName == "delete_appointment")
            goodMessage = "Встреча успешно удалено.";
        else if (methodName == "delete_user")
			goodMessage = "Пользователь успешно удалён.";
		QMessageBox* goodBox =
							new QMessageBox("Операция совершена успешно",
							goodMessage,
							QMessageBox::Information,
							QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
		goodBox->exec();
		delete goodBox;
        if (methodName == "delete_user" || methodName == "add_user" || methodName == "update_user") {
            updateTable("get_users");
		}
        else if (methodName == "add_appointment" || methodName == "update_appointment" || methodName == "delete_appointment") {
            updateTable("get_user_appointments", ui->tableUsers->selectedItems()[0]->text());
		}
	}
	else {
		QString errorMessage;
        if (methodName == "add_user" || methodName == "update_user") {
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

void MainWindow::receiveData(QVariantList data) {
    QString name = sender()->objectName();
    QString methodName;
    if (name == "AddUserDialog")
        methodName = "add_user";
    else if (name == "UpdateUserDialog")
        methodName = "update_user";
    else if (name == "AddAppointmentDialog")
        methodName = "add_appointment";
    else if (name == "UpdateAppointmentDialog")
        methodName = "update_appointment";
    rpc.makeRequest(methodName, data);
}

void MainWindow::prepareData() {
	auto j = sender()->objectName();
	if (sender()->objectName() == "buttonUpdateUser") {
        QVariantMap data = {
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
        QVariantMap data = {
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
        QVariantMap data = {
			{"id", ui->tableUsers->selectedItems()[0]->text()},
			{"name", ui->tableUsers->selectedItems()[1]->text()}
		};
		addAppointment->show();
		emit(sendData(data));
	}
}

void MainWindow::updateTable(QString methodName, QString userId) {
    QVariantList requestData = {};
	if (!userId.isEmpty())
        requestData.append(userId);
    rpc.makeRequest(methodName, requestData);
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
			ui->buttonAddAppointment->setEnabled(true);
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

void MainWindow::dataSlot(QString methodName, QString message, QVector<QMap<QString, QString> > tableData)
{
    if (methodName == "get_users" || methodName == "get_user_appointments") {
		QTableWidget *table;
        if (methodName == "get_users")
			table = ui->tableUsers;
        else if (methodName == "get_user_appointments")
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
        if (methodName == "get_user_appointments" && table->rowCount() > 0)
			ui->buttonDeleteUser->setEnabled(false);
        else if (methodName == "get_user_appointments" && table->rowCount() == 0)
			ui->buttonDeleteUser->setEnabled(true);
        if (methodName == "get_user_appointments")
			ui->buttonAddAppointment->setEnabled(true);
	}
	else
        showMessage(methodName, message);
}

void MainWindow::deleteRow()
{
	QString action, itemId;
	if (sender()->objectName() == "buttonDeleteUser") {
        action = "delete_user";
		itemId = ui->tableUsers->selectedItems()[0]->text();
	}
	else {
        action = "delete_appointment";
		itemId = ui->tableAppointments->selectedItems()[0]->text();
	}
    QVariantList data = {
        itemId
	};
    rpc.makeRequest(action, data);
}
