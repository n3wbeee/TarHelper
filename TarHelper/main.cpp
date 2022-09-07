#include "TarHelperMain.h"
#include "screenOCR.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	ScreenOCR screenOCR;
	TarHelperMain mainWidget;
	mainWidget.show();
	return a.exec();
}