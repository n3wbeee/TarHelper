#include "TarHelperMain.h"
#include "enquiry.h"
#include "maps.h" 

TarHelperMain::TarHelperMain(QWidget* parent)
    : QWidget(parent) {
    ui.setupUi(this);

    setWindowTitle("TarHelper");
    //setWindowFlags(Qt::FramelessWindowHint);

    Maps *widget = new Maps;
    ui.horizontalLayout->addWidget(widget);
    widget->show();
}