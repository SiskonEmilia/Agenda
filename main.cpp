#include "loginwindow.h"
#include "mainwindow.h"
#include <QApplication>
#include <QAnimationDriver>
#include <QPropertyAnimation>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LoginWindow login;
	MainWindow mainWin;

	QPropertyAnimation *animation = new QPropertyAnimation(&login, "windowOpacity");
	animation->setDuration(1000);
	animation->setStartValue(0);
	animation->setEndValue(1);
	animation->start();
	login.show(); // a simple animation

	QObject::connect(&login, SIGNAL(sendLogin()), &mainWin, SLOT(receiveLogin()));
	QObject::connect(&mainWin, SIGNAL(sendLogout()), &login, SLOT(receiveLogout()));

    return a.exec();
}
