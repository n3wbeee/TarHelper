#include "qnetworkaccessmanager.h"
#include "qnetworkreply.h"
#include "qjsondocument.h"
#include "qjsonobject.h"
#include "qtextcodec.h"
#include "qjsonarray.h"
#include "qjsonvalue.h"
#include "screenOCR.h"
#include "qscreen.h"
#include "qbuffer.h"
#include "qpixmap.h"
#include "qcursor.h"
#include "qwidget.h"
#include "qtimer.h"
#include "qevent.h"

screenOCR::screenOCR(QWidget *parent)
	: QWidget(parent), replyAPI(NULL)
{
	ui.setupUi(this);
	networkManager = new QNetworkAccessManager(this);

	setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::SubWindow); //无边框，始终置顶，不显示在任务栏
	setAttribute(Qt::WA_TranslucentBackground);

	QTimer* timer;
	timer = new QTimer(this);
	timer->start(5000);

	connect(timer, &QTimer::timeout, [=]() {
		screenshot();
		timer->stop();
		delete timer;
	});
}

screenOCR::~screenOCR()
{
	delete networkManager;
}

QString screenOCR::replyFinishedOCR(QNetworkReply* replyOCR) {
	QString replyOCRText = replyOCR->readAll();
	QJsonDocument replyJson = QJsonDocument::fromJson(replyOCRText.toUtf8());
	QJsonObject jsonObj = replyJson.object();
	QJsonValue jsonValue = jsonObj.value("words_result");
	QJsonArray jsonArray = jsonValue.toArray();
	QJsonValue wordsValue = jsonArray.first();
	QJsonObject wordsObj = wordsValue.toObject();
	QJsonValue words = wordsObj.value("words");
	QString wordsStr = words.toString();	//套娃 我都不知道写啥注释了 别动这块就行了

	qDebug() << "\n";
	qDebug() << wordsStr;

	qDebug() << replyOCRText;

	replyOCR->abort();
	replyOCR->deleteLater();

	return wordsStr;
}

void screenOCR::screenshot() {
	QPoint	posPoint = QCursor::pos();	//获取指针位置
	qDebug() << posPoint;

	QScreen* screen = QGuiApplication::primaryScreen();
	QPixmap pixmap = screen->grabWindow(0, posPoint.x() + 14, posPoint.y() - 42, 300, 30);	//根据指针位置截图
	QBuffer buffer;
	buffer.open(QIODevice::WriteOnly);
	pixmap.save(&buffer, "png");
	auto const base64 = buffer.data().toBase64();	//将图片转换为base64
	pixmap.save("C:/Users/10637_c4lx35f/Desktop/TarHelperOCR/test.png");
	/*配置POST请求*/
	QNetworkRequest req;
	QUrl url = "https://aip.baidubce.com/rest/2.0/ocr/v1/general_basic";
	QString access_token = "24.642429a0b17f786950c2c65de79063f2.2592000.1658895930.282335-26556990";
	url = url.toString() + "?access_token=" + access_token;
	req.setUrl(url);
	req.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
	QByteArray data;
	data.append("image=");
	data.append(QUrl::toPercentEncoding(base64));

	QNetworkReply* replyOCR = networkManager->post(req, data);
	connect(replyOCR, &QNetworkReply::finished, [=]() {
		enquiryPrice(screenOCR::replyFinishedOCR(replyOCR).toLower());
	});  //当请求完成时，由返回值询价
}

void screenOCR::enquiryPrice(QString itemName) {
	QString url = "https://mp.soulofcinder.xyz/eft/api/mainSearch?tag=&page_num=1&page_size=1&item_name=";
	url = url + itemName;
	QNetworkRequest req;
	req.setUrl(QUrl(url));
	replyAPI = networkManager->get(req);  //HTML请求

	connect(replyAPI, &QNetworkReply::finished, this, &screenOCR::replyFinishedAPI);  //当请求完成时，执行槽函数
}

void screenOCR::replyFinishedAPI() {
	QTextCodec* codec = QTextCodec::codecForName("utf8");
	QString replyAPIText = codec->toUnicode(replyAPI->readAll());
	QJsonDocument replyAPIJson = QJsonDocument::fromJson(replyAPIText.toUtf8());
	/*读取Json中Array的每一个Object*/
	QJsonArray jsonArray = replyAPIJson.array();
	for (int objCount = 0; objCount < jsonArray.size(); objCount++) {
		QJsonValue jsonValue = jsonArray.at(objCount);
		QJsonObject jsonObject = jsonValue.toObject();
		/*询价*/
		QJsonValue valuePrice = jsonObject.value("avgDayPrice");
		double valueDouble = valuePrice.toDouble();
		screenOCR::price = QString::number(valueDouble, 'f', 0);

		QJsonValue valuePricePerSlot = jsonObject.value("avgWeekPricePerSlot");
		double valueSlot = valuePricePerSlot.toDouble();
		screenOCR::pricePerSlot = QString::number(valueSlot, 'f', 0);
	} 
	replyAPI->abort();
	replyAPI->deleteLater(); //用户有责任在适当的时候删除 QNetworkreplyAPI 对象
	
	ui.priceText->setText("Price:");
	ui.price->setText(price);
	show();

	QTimer* timer;
	timer = new QTimer(this);
	timer->start(5000);
	 
	move(QCursor::pos());

	connect(timer, &QTimer::timeout, [=]() {
		hide();
		timer->stop();
		delete timer;
	});
}

void screenOCR::mouseMoveEvent(QMouseEvent* event) {
	move(event->pos());
	qDebug() << "MouseMoveEvent";
}

//混个Contribution