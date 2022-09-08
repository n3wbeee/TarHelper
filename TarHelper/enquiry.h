#pragma once

#include <QWidget>
#include <WinSock2.h>
#include "ui_enquiry.h"
#include "qnetworkaccessmanager.h"

class Enquiry : public QWidget {
	Q_OBJECT

public:
	Enquiry(QWidget *parent = Q_NULLPTR);
	~Enquiry();

private slots:
	void on_lineEdit_returnPressed();
	void on_pushButton_clicked();
	void replyFinishedAPI();

private:
	Ui::enquiry ui;
	QNetworkReply* replyAPI;
	QNetworkAccessManager* networkManager;

	void replyFinishedIcon(int i, QNetworkReply* replyIcon);
};