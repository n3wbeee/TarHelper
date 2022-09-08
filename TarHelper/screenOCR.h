#pragma once

#include <QWidget>
#include <QShortCut>
#include "ui_screenOCR.h"
#include "qxtglobalshortcut.h"
#include "qnetworkaccessmanager.h"

class ScreenOCR : public QWidget
{
	Q_OBJECT

public:
	ScreenOCR(QWidget *parent = nullptr);
	~ScreenOCR();

private slots:
	void replyFinishedAPI();

private:
	Ui::screenOCRClass ui;

	QString replyFinishedOCR(QNetworkReply* replyOCR);
	void mouseMoveEvent(QMouseEvent* event);
	void enquiryPrice(QString itemName);
	void screenshot();

	QNetworkAccessManager* networkManager;
	QxtGlobalShortcut* callOut;
	QNetworkReply* replyAPI;
	QString pricePerSlot;
	QString change7d;
	QString cnName;
	QString price;
};
