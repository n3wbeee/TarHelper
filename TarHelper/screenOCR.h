#pragma once

#include <QWidget>
#include "ui_screenOCR.h"
#include "qnetworkaccessmanager.h"

class screenOCR : public QWidget
{
	Q_OBJECT

public:
	screenOCR(QWidget *parent = nullptr);
	~screenOCR();

private slots:
	void replyFinishedAPI();

private:
	Ui::screenOCRClass ui;

	QString replyFinishedOCR(QNetworkReply* replyOCR);
	void mouseMoveEvent(QMouseEvent* event);
	void enquiryPrice(QString itemName);
	void screenshot();

	QNetworkAccessManager* networkManager;
	QNetworkReply* replyAPI;
	QString pricePerSlot;
	QString price;
};
