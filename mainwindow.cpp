#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainWindow),
	timer(new QTimer(this))
{
    ui->setupUi(this);
	timer->start(1000);
	QObject::connect(timer, SIGNAL(timeout()), this, SLOT(timeUpdate()));
	QObject::connect(ui->myList, SIGNAL(currentRowChanged(int)), ui->myStacked, SLOT(setCurrentIndex(int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::timeUpdate(){
	ui->presentTime->setText(
		QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
}

void MainWindow::receiveLogin() {
	initialRefresh();
	ui->myList->setCurrentRow(7);
	this->show();
}

void MainWindow::on_quitButton_clicked() {
	int flag = QMessageBox::question(this, str2qstr(string("�˳�����")), str2qstr(string("�Ƿ��˳������������޸Ľ����Զ�����")), QMessageBox::Yes, QMessageBox::No);
	if (flag != 16384)
		return;
	else {
		dataService.saveNow();
		exit(0);
	}
}

void MainWindow::closeEvent(QCloseEvent* event) {
	int flag = QMessageBox::question(this, str2qstr(string("�˳�����")), str2qstr(string("�Ƿ��˳������������޸Ľ����Զ�����")), QMessageBox::Yes, QMessageBox::No);
	if (flag != 16384)
		event->ignore();
	else {
		event->accept();
		dataService.saveNow();
		exit(0);
	}
}

void MainWindow::on_logoutButton_clicked() {
	presentPass = presentUser = "";
	uiClear();
	this->hide();
	emit sendLogout();
}

void MainWindow::on_deleteButton_clicked() {
	if (!dataService.deleteUser(presentUser, qstr2str(ui->passHolder->text()))) {
		ui->passHolder->clear();
		QMessageBox::warning(this, str2qstr("Error"), str2qstr("������󣬲���ʧ��"));
		return;
	}
	ui->passHolder->clear();
	QMessageBox::information(this, str2qstr("Notice"), str2qstr("�����ɹ����˻��Ѿ�ɾ��"));
	presentPass = presentUser = "";
	this->hide();
	emit sendLogout();
}

void MainWindow::uiClear() {
	ui->titleLine->clear();
	ui->participator->clear();
	ui->passHolder->clear();
	ui->titleIn->clear();
	ui->toAdd->clear();
	ui->toRemove->clear();
}

void MainWindow::on_creatButton_clicked() {
	string title = qstr2str(ui->titleLine->text());
	
	QDateTime sta = ui->startTime->dateTime(),
		end = ui->endTime->dateTime();
	
	string mySta(qstr2str(sta.toString("yyyy-MM-dd/hh:mm"))),
		myEnd(qstr2str(end.toString("yyyy-MM-dd/hh:mm")));

	string part = qstr2str(ui->participator->toPlainText());

	uiClear();
	
	stringstream temp;
	vector<string> toPat;
	temp << part;
	while (getline(temp, part)) {
		toPat.push_back(part);
	}
	if (title == "")
	{
		QMessageBox::warning(this, str2qstr(string("Error")), str2qstr(string("���ⲻ��Ϊ��")));
		return;
	}

	if (title.find('\"') != -1)
	{
		QMessageBox::warning(this, str2qstr(string("Error")), str2qstr(string("�����в�������Ӣ������")));
		return;
	}

	if (dataService.createMeeting(presentUser, title, mySta, myEnd, toPat)) {
		QMessageBox::information(this, str2qstr("Notice"), str2qstr("�����ɹ�"));
		initialRefresh();
	}
	else
		QMessageBox::warning(this, str2qstr("Error"), str2qstr("����ʧ�ܣ�������Ǳ����ͻ������ʱ���ͻ�򲻺���Ĳ����ߵ��µ�"));


}

void MainWindow::initialRefresh() {
	refreshUserTabel(ui->userTabel, dataService.listAllUsers()); // All user list
	refreshMeetingTabel(ui->meetingTabelMan, dataService.listAllSponsorMeetings(presentUser));// My meeting list
	refreshMeetingTabel(ui->meetingTabelQuit, dataService.listAllParticipateMeetings(presentUser));// My meeting list
	refreshMeetingTabel(ui->meetingTabelView, dataService.listAllMeetings(presentUser));// My meeting list
	ui->meetingTabelSea->setRowCount(0);
	
}

void MainWindow::refreshUserTabel(QTableWidget *toDisplay, std::list<User> &toShow) {
	toDisplay->setRowCount(toShow.size());
	int counter = 0;
	for (auto i : toShow) {
		toDisplay->setItem(counter, 0, new QTableWidgetItem(str2qstr(i.getName())));
		toDisplay->setItem(counter, 1, new QTableWidgetItem(str2qstr(i.getEmail())));
		toDisplay->setItem(counter++, 2, new QTableWidgetItem(str2qstr(i.getPhone())));
	}
}

void MainWindow::refreshMeetingTabel(QTableWidget *toDisplay, std::list<Meeting> &toShow) {
	toDisplay->setRowCount(toShow.size());
	int counter = 0;
	string participator;
	for (auto i : toShow) {
		toDisplay->setItem(counter, 0, new QTableWidgetItem(str2qstr(i.getTitle())));
		toDisplay->setItem(counter, 1, new QTableWidgetItem(str2qstr(i.getSponsor())));
		toDisplay->setItem(counter, 2, new QTableWidgetItem(str2qstr(Date::dateToString(i.getStartDate()))));
		toDisplay->setItem(counter, 3, new QTableWidgetItem(str2qstr(Date::dateToString(i.getEndDate()))));
		participator.clear();
		auto partici = i.getParticipator();
		int pCounter = 0;
		for (auto t : partici) {
			if (pCounter++ != 0) {
				participator += ", ";
			}
			participator += t;
		}
		toDisplay->setItem(counter++, 4, new QTableWidgetItem(str2qstr(participator)));
	}
}

void MainWindow::on_searchByTitle_clicked() {
	string titleIn = qstr2str(ui->titleIn->text());
	uiClear();
	refreshMeetingTabel(ui->meetingTabelSea, dataService.meetingQuery(presentUser, titleIn));
}

void MainWindow::on_searchByDate_clicked() {
	QDateTime sta = ui->staTimeE->dateTime(),
		end = ui->endTimeE->dateTime();

	string mySta(qstr2str(sta.toString("yyyy-MM-dd/hh:mm"))),
		myEnd(qstr2str(end.toString("yyyy-MM-dd/hh:mm")));
	uiClear();
	refreshMeetingTabel(ui->meetingTabelSea, dataService.meetingQuery(presentUser, mySta, myEnd));
}

string MainWindow::getCurrentTitle() {
	int index = ui->meetingTabelMan->currentRow();
	if (index == -1)
		throw string("");
	string res = qstr2str(ui->meetingTabelMan->item(index, 0)->text());
	return res;
}

string MainWindow::getQuitTitle() {
	int index = ui->meetingTabelQuit->currentRow();
	if (index == -1)
		throw string("");
	string res = qstr2str(ui->meetingTabelQuit->item(index, 0)->text());
	return res;
}

void MainWindow::on_removeButton_clicked() {
	try {
		string title = getCurrentTitle(),
			userName = qstr2str(ui->toRemove->text());
		if (dataService.removeMeetingParticipator(presentUser, title, userName)) {
			QMessageBox::information(this, str2qstr("Notice"), str2qstr("�����ɹ�"));
		}
		else {
			QMessageBox::warning(this, str2qstr("Error"), str2qstr("�Ƴ�ʧ�ܣ������߲�����"));
		}
		initialRefresh();
	}
	catch (string error) {
		QMessageBox::warning(this, str2qstr("Warning"), str2qstr("����ѡ����Ҫ�����Ļ���"));
	}
}

void MainWindow::on_addButton_clicked() {
	try {
		string title = getCurrentTitle(),
			userName = qstr2str(ui->toAdd->text());
		if (dataService.addMeetingParticipator(presentUser, title, userName)) {
			QMessageBox::information(this, str2qstr("Notice"), str2qstr("�����ɹ�"));
		}
		else {
			QMessageBox::warning(this, str2qstr("Error"), str2qstr("�Ƴ�ʧ�ܣ�����������û��������ڣ��û�ʱ���ͻ���ظ���ӵ���"));
		}
		initialRefresh();
	}
	catch (string error) {
		QMessageBox::warning(this, str2qstr("Warning"), str2qstr("����ѡ����Ҫ�����Ļ���"));
	}
}

void MainWindow::on_deleteSelect_clicked() {
	try {
		string title = getCurrentTitle();
		if (dataService.deleteMeeting(presentUser, title)) {
			QMessageBox::information(this, str2qstr("Notice"), str2qstr("�����ɹ�"));
		}
		else {
			QMessageBox::warning(this, str2qstr("Error"), str2qstr("����ʧ�ܣ�δ֪����"));
		}
		initialRefresh();
	}
	catch (string error) {
		QMessageBox::warning(this, str2qstr("Warning"), str2qstr("����ѡ����Ҫ�����Ļ���"));
	}
}

void MainWindow::on_deleteAll_clicked() {
	if (dataService.deleteAllMeetings(presentUser)) {
		QMessageBox::information(this, str2qstr("Notice"), str2qstr("�����ɹ�"));
	}
	else {
		QMessageBox::warning(this, str2qstr("Error"), str2qstr("����û�д����κλ���"));
	}
	initialRefresh();
}

void MainWindow::on_meetingQuit_clicked() {
	try {
		string title = getQuitTitle();
		if (dataService.quitMeeting(presentUser, title)) {
			QMessageBox::information(this, str2qstr("Notice"), str2qstr("�����ɹ�"));
		}
		else {
			QMessageBox::warning(this, str2qstr("Error"), str2qstr("����ʧ�ܣ�δ֪����"));
		}
		initialRefresh();
	}
	catch (string error) {
		QMessageBox::warning(this, str2qstr("Warning"), str2qstr("����ѡ����Ҫ�˳��Ļ���"));
	}
}