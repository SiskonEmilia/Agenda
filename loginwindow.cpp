#include "loginwindow.h"
#include "ui_loginwindow.h"

LoginWindow::LoginWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::on_quitBotton_clicked()
{
	int flag = QMessageBox::question(this, str2qstr(string("�˳�����")), str2qstr(string("�Ƿ��˳������������޸Ľ����Զ�����")), QMessageBox::Yes, QMessageBox::No);
	if (flag != 16384)
		return;
	else {
		dataService.saveNow();
		exit(0);
	}
}

void LoginWindow::closeEvent(QCloseEvent *event)
{
	int flag = QMessageBox::question(this, str2qstr(string("�˳�����")), str2qstr(string("�Ƿ��˳������������޸Ľ����Զ�����")), QMessageBox::Yes, QMessageBox::No);
	if (flag != 16384)
		event->ignore();
	else{
		event->accept();
		dataService.saveNow();
		exit(0);
	}
}

void LoginWindow::clearLines() {
	ui->userName->clear();
	ui->passWord->clear();
	ui->phone->clear();
	ui->email->clear();
}

void LoginWindow::on_loginButton_clicked()
{
	string username = qstr2str(ui->userName->text()),
		password = qstr2str(ui->passWord->text());

	clearLines();

	if (username == "")
	{
		QMessageBox::warning(this, str2qstr(string("Error")), str2qstr(string("�û�������Ϊ��")));
		return;
	}

	if (dataService.userLogIn(username, password)) {
		presentUser = username;
		presentPass = password;
		this->hide();
		emit sendLogin();
		return;
	}

	QMessageBox::warning(this, str2qstr(string("Error")), str2qstr(string("�����ڵ��û������������")));
}

void LoginWindow::on_registerButton_clicked()
{
	string username = qstr2str(ui->userName->text()),
		password = qstr2str(ui->passWord->text()),
		email = qstr2str(ui->email->text()),
		phone = qstr2str(ui->phone->text());

	clearLines();

	if (username == "")
	{
		QMessageBox::warning(this, str2qstr(string("Error")), str2qstr(string("�û�������Ϊ��")));
		return;
	}
	if (username.find('\"') != -1 || username.find(',') != -1)
	{
		QMessageBox::warning(this, str2qstr(string("Error")), str2qstr(string("�û����в����������Ż򶺺�")));
		return;
	}
	if (password.size() < 8 || password.size() > 20)
	{
		QMessageBox::warning(this, str2qstr(string("Error")), str2qstr(string("����λ������С��8�����20")));
		return;
	}
	if (password.find('\"') != -1)
	{
		QMessageBox::warning(this, str2qstr(string("Error")), str2qstr(string("�����в�����������")));
		return;
	}
	if (email == "")
	{
		QMessageBox::warning(this, str2qstr(string("Error")), str2qstr(string("���䲻��Ϊ��")));
		return;
	}
	if (email.find('\"') != -1)
	{
		QMessageBox::warning(this, str2qstr(string("Error")), str2qstr(string("�����в�����������")));
		return;
	}
	if (phone == "")
	{
		QMessageBox::warning(this, str2qstr(string("Error")), str2qstr(string("�绰���벻��Ϊ��")));
		return;
	}
	if (phone.find('\"') != -1)
	{
		QMessageBox::warning(this, str2qstr(string("Error")), str2qstr(string("�绰�����в�����������")));
		return;
	}

	if (dataService.userRegister(username, password, email, phone)) {
		QMessageBox::information(this, str2qstr(string("Notice")), str2qstr(string("ע��ɹ�")));
		return;
	}
	
	QMessageBox::warning(this, str2qstr(string("Error")), str2qstr(string("�˻����Ѿ�����")));

}

void LoginWindow::receiveLogout() {
	this->show();
}