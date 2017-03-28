#ifndef ADDUSERDIALOG_H
#define ADDUSERDIALOG_H

#include <QDialog>
#include <QMap>
#include <QPushButton>
#include <QRegExp>
#include <QVariant>

namespace Ui {
class AddUserDialog;
}

class AddUserDialog : public QDialog
{
	Q_OBJECT

public:
	explicit AddUserDialog(QWidget *parent = 0);
	~AddUserDialog();

signals:
    sendData(QVariantList);

private slots:
	void checkFields();
	void on_buttonBox_accepted();
	void clearFields();

private:
	Ui::AddUserDialog *ui;
};

#endif // ADDUSERDIALOG_H
