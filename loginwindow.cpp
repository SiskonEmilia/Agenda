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
	int flag = QMessageBox::question(this, str2qstr(string("退出程序")), str2qstr(string("是否退出？您所做的修改将会自动保存")), QMessageBox::Yes, QMessageBox::No);
	if (flag != 16384)
		return;
	else {
		dataService.saveNow();
		exit(0);
	}
}

void LoginWindow::closeEvent(QCloseEvent *event)
{
	int flag = QMessageBox::question(this, str2qstr(string("退出程序")), str2qstr(string("是否退出？您所做的修改将会自动保存")), QMessageBox::Yes, QMessageBox::No);
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
		QMessageBox::warning(this, str2qstr(string("Error")), str2qstr(string("用户名不能为空")));
		return;
	}

	if (dataService.userLogIn(username, password)) {
		presentUser = username;
		presentPass = password;
		this->hide();
		emit sendLogin();
		return;
	}

	QMessageBox::warning(this, str2qstr(string("Error")), str2qstr(string("不存在的用户名或密码错误")));
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
		QMessageBox::warning(this, str2qstr(string("Error")), str2qstr(string("用户名不能为空")));
		return;
	}
	if (username.find('\"') != -1 || username.find(',') != -1)
	{
		QMessageBox::warning(this, str2qstr(string("Error")), str2qstr(string("用户名中不允许含有引号或逗号")));
		return;
	}
	if (password.size() < 8 || password.size() > 20)
	{
		QMessageBox::warning(this, str2qstr(string("Error")), str2qstr(string("密码位数不得小于8或大于20")));
		return;
	}
	if (password.find('\"') != -1)
	{
		QMessageBox::warning(this, str2qstr(string("Error")), str2qstr(string("密码中不允许含有引号")));
		return;
	}
	if (email == "")
	{
		QMessageBox::warning(this, str2qstr(string("Error")), str2qstr(string("邮箱不能为空")));
		return;
	}
	if (email.find('\"') != -1)
	{
		QMessageBox::warning(this, str2qstr(string("Error")), str2qstr(string("邮箱中不允许含有引号")));
		return;
	}
	if (phone == "")
	{
		QMessageBox::warning(this, str2qstr(string("Error")), str2qstr(string("电话号码不能为空")));
		return;
	}
	if (phone.find('\"') != -1)
	{
		QMessageBox::warning(this, str2qstr(string("Error")), str2qstr(string("电话号码中不允许含有引号")));
		return;
	}

	if (dataService.userRegister(username, password, email, phone)) {
		QMessageBox::information(this, str2qstr(string("Notice")), str2qstr(string("注册成功")));
		return;
	}
	
	QMessageBox::warning(this, str2qstr(string("Error")), str2qstr(string("账户名已经存在")));

}

void LoginWindow::receiveLogout() {
	this->show();
}