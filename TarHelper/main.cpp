#include "TarHelperMain.h"
#include "Maps.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Maps maps;
	maps.show();
	return a.exec();
}