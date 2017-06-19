#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDialog>
#include <QTimer>
#include <QDateTime>
#include <QCloseEvent>
#include <QMessageBox>
#include <QTableWidget>
#include <stdexcept>
#include <sstream>
#include <list>
#include <vector>
#include "AgendaService.hpp"

using std::string;
using std::stringstream;
using std::vector;

namespace Ui {
class MainWindow;
}

class MainWindow : public QDialog
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
	void refreshUserTabel(QTableWidget *toDisplay, std::list<User> &toShow);
	void refreshMeetingTabel(QTableWidget *toDisplay, std::list<Meeting> &toShow);
	void initialRefresh();

private:
    Ui::MainWindow *ui;
	QTimer *timer;
	void closeEvent(QCloseEvent* event);
	void uiClear();

private slots:
	void timeUpdate();
	void receiveLogin();
	void on_quitButton_clicked();
	void on_logoutButton_clicked();
	void on_deleteButton_clicked();// delete User
	void on_creatButton_clicked();
	void on_searchByTitle_clicked();
	void on_searchByDate_clicked();
	void on_removeButton_clicked();
	void on_addButton_clicked();
	void on_deleteSelect_clicked();
	void on_deleteAll_clicked();
	void on_meetingQuit_clicked();
	string getCurrentTitle();
	string getQuitTitle();


signals:
	void sendLogout();
};

#endif // MAINWINDOW_H
