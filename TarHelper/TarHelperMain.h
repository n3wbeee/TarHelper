#pragma once

#include <QtWidgets/QWidget>
#include "ui_TarHelperMain.h"

class TarHelperMain : public QWidget {
    Q_OBJECT

public:
    TarHelperMain(QWidget *parent = Q_NULLPTR);

private:
    Ui::TarHelperMainClass ui;
};
