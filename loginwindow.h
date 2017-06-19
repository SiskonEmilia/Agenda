#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QDialog>  
#include <QLabel>  
#include <QListWidget>  
#include <QStackedWidget>
#include <QCloseEvent>
#include "AgendaService.hpp"

namespace Ui {
class LoginWindow;
}

class LoginWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = 0);
    ~LoginWindow();

private slots:
    void on_quitBotton_clicked();

    void on_loginButton_clicked();

    void on_registerButton_clicked();

	void closeEvent(QCloseEvent *event);

	void receiveLogout();

signals:
	void sendLogin();

private:
    Ui::LoginWindow *ui;
	void clearLines();
};



#endif // LOGINWINDOW_H
