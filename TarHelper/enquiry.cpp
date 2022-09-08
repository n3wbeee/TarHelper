#include <QShortCut>
#include "enquiry.h"
#include "qdebug.h"
#include "qlabel.h"
#include "qbuffer.h"
#include "qscreen.h"
#include "qtextcodec.h"
#include "qjsonarray.h"
#include "qmessagebox.h"
#include "qjsonobject.h"
#include "qtablewidget.h"
#include "qjsondocument.h"
#include "qnetworkreply.h"
#include "qxtglobalshortcut.h"

Enquiry::Enquiry(QWidget* parent)
	: QWidget(parent), replyAPI(NULL) {
	ui.setupUi(this);

	ui.tableWidget->setColumnCount(3);
	ui.tableWidget->setColumnWidth(0, 96);

	networkManager = new QNetworkAccessManager(this);
}

Enquiry::~Enquiry() {	
	delete networkManager;
}

void Enquiry::replyFinishedAPI() {
	if (replyAPI->error()) {
		qDebug() << replyAPI->errorString();
		replyAPI->deleteLater();
		QMessageBox::critical(this, "Network Error", replyAPI->errorString());
		return;
	}
	int statusCode = replyAPI->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
	qDebug() << "statusCode:" << statusCode;
	if (statusCode != 200) {
		QMessageBox::critical(this, "Network Error", static_cast<QString>(statusCode));
		return;
	}
	/*清空tableWidget*/
	ui.tableWidget->clear();
	/*将获得的返回值转换为Json并解析*/
	QTextCodec* codec = QTextCodec::codecForName("utf8");
	QString replyAPIText = codec->toUnicode(replyAPI->readAll());
	QJsonDocument replyAPIJson = QJsonDocument::fromJson(replyAPIText.toUtf8());
	/*读取Json中Array的每一个Object*/
	QJsonArray jsonArray = replyAPIJson.array();
	ui.tableWidget->setRowCount(jsonArray.size());
	for (int objCount = 0; objCount < jsonArray.size(); objCount++) {
		QJsonValue jsonValue = jsonArray.at(objCount);
		QJsonObject jsonObject = jsonValue.toObject();
		ui.tableWidget->setRowHeight(objCount, 96);
		/*询名*/
		QJsonValue valueName = jsonObject.value("cnName");
		QString cnName = valueName.toString();
		qDebug() << cnName;

		QTableWidgetItem* nameItem = new QTableWidgetItem;
		nameItem->setText(cnName);
		nameItem->setTextColor("#B8B6B4");
		//nameItem->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter); //居中
		ui.tableWidget->setItem(objCount, 1, nameItem);
		/*询价*/
		QJsonValue valuePrice = jsonObject.value("avgDayPrice");
		double valueDouble = valuePrice.toDouble();
		QString valueStr = QString::number(valueDouble, 'f', 0);

		QTableWidgetItem* valueItem = new QTableWidgetItem;
		valueItem->setText(valueStr);
		valueItem->setTextColor("#B8B6B4");
		ui.tableWidget->setItem(objCount, 2, valueItem);
		/*icon*/
		QJsonValue value = jsonObject.value("wikiIcon");
		QNetworkRequest req;
		req.setUrl(QUrl(value.toString()));
		/*创建一个QNetworkReply对象，作为参数传递给函数*/
		QNetworkReply* replyIcon = networkManager->get(req);  //HTML请求

		connect(replyIcon, &QNetworkReply::finished, [=]() {
			Enquiry::replyFinishedIcon(objCount, replyIcon);
		});  //当请求完成时，执行函数配置icon
	}
	replyAPI->abort();
	replyAPI->deleteLater(); //用户有责任在适当的时候删除 QNetworkreplyAPI 对象
}

void Enquiry::replyFinishedIcon(int row, QNetworkReply* replyIcon) {
	/*配置Icon*/
	QLabel* icon = new QLabel;
	QPixmap pixmap;

	pixmap.loadFromData(replyIcon->readAll());
	icon->setPixmap(pixmap.scaled(72, 72, Qt::KeepAspectRatio, Qt::SmoothTransformation)); //图像缩放
	icon->setAlignment(Qt::AlignCenter);
	if (row%2){
		icon->setStyleSheet("background: #323130");	//隔行高亮
	}
	ui.tableWidget->setCellWidget(row, 0, icon);
	
	replyIcon->abort();
	replyIcon->deleteLater();
}

void Enquiry::on_lineEdit_returnPressed() {
	QString url = "https://mp.soulofcinder.xyz/eft/api/mainSearch?tag=&page_num=1&page_size=300&item_name=";
	url = url + ui.lineEdit->text();
	QNetworkRequest req;
	req.setUrl(QUrl(url));
	replyAPI = networkManager->get(req);  //HTML请求

	connect(replyAPI, &QNetworkReply::finished, this, &Enquiry::replyFinishedAPI);  //当请求完成时，执行槽函数
}

void Enquiry::on_pushButton_clicked() {
	/*debug Here*/
};