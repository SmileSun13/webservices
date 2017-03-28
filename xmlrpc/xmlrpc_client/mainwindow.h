#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "xmlrpcclass.h"
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
#include <QVariant>

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
    sendData(QVariantMap);

private slots:
    void dataSlot(QString methodName, QString message, QVector<QMap<QString, QString>> tableData);
	void deleteRow();
	void checkSelection();
    void receiveData(QVariantList data);
	void prepareData();

private:
    Ui::MainWindow *ui;
    XmlRPCClass rpc;
	AddUserDialog *addUser;
	UpdateUserDialog *updateUser;
	AddAppointmentDialog *addAppointment;
	UpdateAppointmentDialog *updateAppointment;
    void updateTable(QString methodName, QString userId = "");
	void setupConnections();
    void showMessage(QString methodName, QString message);
};

#endif // MAINWINDOW_H
