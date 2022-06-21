#include "TarHelperMain.h"
#include "enquiry.h"
#include "maps.h" 

TarHelperMain::TarHelperMain(QWidget* parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    setWindowTitle("TarHelper");
    setWindowFlags(Qt::FramelessWindowHint);

    Enquiry *enquiry = new Enquiry;
    ui.horizontalLayout->addWidget(enquiry);
    enquiry->show();
}