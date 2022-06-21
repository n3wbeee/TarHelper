#include "TarHelperMain.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	TarHelperMain mainWidget;
	mainWidget.show();
	return a.exec();
}