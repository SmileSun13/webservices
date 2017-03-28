#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "networkclass.h"
#include "xmlclass.h"
#include "adduserdialog.h"
#include "updateuserdialog.h"
#include "addappointmentdialog.h"
#include "updateappointmentdialog.h"
#include <QLabel>
#include <QFormLayout>
#include <QTextEdit>
#include <QLineEdit>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QFormLayout>
#include <QTableWidget>
#include <QMessageBox>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
	sendData(QMap<QString, QString>);

private slots:
	void dataSlot(QByteArray data);
	void deleteRow();
	void checkSelection();
	void receiveData(QMap<QString, QString> data);
	void prepareData();

private:
    Ui::MainWindow *ui;
	NetworkClass net;
	XmlClass xml;
	AddUserDialog *addUser;
	UpdateUserDialog *updateUser;
	AddAppointmentDialog *addAppointment;
	UpdateAppointmentDialog *updateAppointment;
	void updateTable(QString actionType, QString userId = "");
	void setupConnections();
	void showMessage(QMap<QString, QString> tags);
};

#endif // MAINWINDOW_H
