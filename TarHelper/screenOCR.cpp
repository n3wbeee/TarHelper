#include "qnetworkaccessmanager.h"
#include "qxtglobalshortcut.h"
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

ScreenOCR::ScreenOCR(QWidget *parent)
	: QWidget(parent), replyAPI(NULL)
{
	ui.setupUi(this);
	networkManager = new QNetworkAccessManager(this);

	setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::SubWindow); //无边框，始终置顶，不显示在任务栏
	setAttribute(Qt::WA_TranslucentBackground);

	callOut = new QxtGlobalShortcut(this);  //配置全局热键
	callOut->setShortcut(Qt::CTRL + Qt::Key_F);
	/*如果调出热键时窗口已经隐藏，执行询价；若窗口显示中，隐藏窗口*/
	connect(callOut, &QxtGlobalShortcut::activated, [&]() {
		qDebug() << "function called!";
		if (isHidden() == true) {
			screenshot();
		}
		else {
			hide();
		}
	});
}

ScreenOCR::~ScreenOCR()
{
	delete networkManager;
	delete callOut;
}

QString ScreenOCR::replyFinishedOCR(QNetworkReply* replyOCR) {
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

void ScreenOCR::screenshot() {
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
	QString access_token = "24.5a72d149ff8be6939ce3ebb66cf8ea25.2592000.1665144271.282335-26556990";
	url = url.toString() + "?access_token=" + access_token;
	req.setUrl(url);
	req.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
	QByteArray data;
	data.append("image=");
	data.append(QUrl::toPercentEncoding(base64));

	QNetworkReply* replyOCR = networkManager->post(req, data);
	connect(replyOCR, &QNetworkReply::finished, [=]() {
		enquiryPrice(ScreenOCR::replyFinishedOCR(replyOCR).toLower());
	});  //当请求完成时，由返回值询价
}

void ScreenOCR::enquiryPrice(QString itemName) {
	QString url = "https://mp.soulofcinder.xyz/eft/api/mainSearch?tag=&page_num=1&page_size=1&item_name=";
	url = url + itemName;
	QNetworkRequest req;
	req.setUrl(QUrl(url));
	replyAPI = networkManager->get(req);  //HTML请求

	connect(replyAPI, &QNetworkReply::finished, this, &ScreenOCR::replyFinishedAPI);  //当请求完成时，执行槽函数
}

void ScreenOCR::replyFinishedAPI() {
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
		ScreenOCR::price = QString::number(valueDouble, 'f', 0);

		QJsonValue valueChange7d = jsonObject.value("change7d");
		double valueChange = valueChange7d.toDouble();
		ScreenOCR::change7d = QString::number(valueChange, 'f', 2);
		ScreenOCR::change7d = ScreenOCR::change7d + "%";

		QJsonValue valuePricePerSlot = jsonObject.value("avgDayPricePerSlot");
		double valueSlot = valuePricePerSlot.toDouble();
		ScreenOCR::pricePerSlot = QString::number(valueSlot, 'f', 0);

		QJsonValue valueName = jsonObject.value("cnShortName");
		ScreenOCR::cnName = valueName.toString();
		qDebug() << cnName;
	}
	replyAPI->abort();
	replyAPI->deleteLater(); //用户有责任在适当的时候删除 QNetworkreplyAPI 对象

	ui.name->setText(cnName);
	ui.priceNow->setText(price);
	ui.risefall->setText(change7d);
	ui.pricePerSlot->setText(pricePerSlot);
	move(QCursor::pos());
	show();
}

void ScreenOCR::mouseMoveEvent(QMouseEvent* event) {
	move(event->pos());
	qDebug() << "MouseMoveEvent";
}