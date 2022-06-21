#pragma once

#include <QWidget>
#include <WinSock2.h>
#include "ui_enquiry.h"
#include "qnetworkaccessmanager.h"

class Enquiry : public QWidget
{
	Q_OBJECT

public:
	Enquiry(QWidget *parent = Q_NULLPTR);
	~Enquiry();

private slots:
	void on_lineEdit_returnPressed();
	void replyFinished();

private:
	Ui::enquiry ui;
	QNetworkRequest req;
	QNetworkReply* reply;
	QNetworkAccessManager *networkManager;
};
