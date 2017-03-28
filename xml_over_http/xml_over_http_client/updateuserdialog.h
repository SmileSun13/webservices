#ifndef UPDATEUSERDIALOG_H
#define UPDATEUSERDIALOG_H

#include <QDialog>
#include <QMap>
#include <QPushButton>
#include <QRegExp>

namespace Ui {
class UpdateUserDialog;
}

class UpdateUserDialog : public QDialog
{
	Q_OBJECT

public:
	explicit UpdateUserDialog(QWidget *parent = 0);
	~UpdateUserDialog();

signals:
	sendData(QMap<QString, QString>);

private slots:
	void on_buttonBox_accepted();
	void receiveData(QMap<QString, QString> data);
	void checkFields();
	void clearFields();

private:
	Ui::UpdateUserDialog *ui;
	QString userId;
};

#endif // UPDATEUSERDIALOG_H
