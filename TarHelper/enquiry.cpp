#include "enquiry.h"
#include "qdebug.h"
#include "qtextcodec.h"
#include "qjsonarray.h"
#include "qjsonobject.h"
#include "qjsondocument.h"
#include "qnetworkreply.h"

Enquiry::Enquiry(QWidget* parent)
	: QWidget(parent) 
{
	ui.setupUi(this);

	req.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
	req.setRawHeader("Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9");
	req.setRawHeader("User-Agent", "Mozilla/5.0 (Windows NT 10.0) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/86.0.4240.198 Safari/537.36");

	networkManager = new QNetworkAccessManager(this);
}

Enquiry::~Enquiry()
{	
	delete networkManager;
}

void Enquiry::replyFinished(){
	/*将获得的返回值转换为Json并解析*/
	QTextCodec* codec = QTextCodec::codecForName("utf8");
	QString replyText = codec->toUnicode(reply->readAll());
	QJsonDocument replyJson = QJsonDocument::fromJson(replyText.toUtf8());
	/*读取Json中Array的第一个Object*/
	QJsonArray jsonArray = replyJson.array();
	QJsonValue jsonValue = jsonArray.first();
	QJsonObject jsonObject = jsonValue.toObject();
	/*遍历Object的每个key，找到需要的key*/
	QStringList keys = jsonObject.keys();
	for (int i = 0; i < keys.size(); ++i) {
		QString key = keys.at(i);
		if (key == "cnName") {
			QJsonValue value = jsonObject.value(key);
			qDebug() << key << ":" << value.toString();
			ui.listWidget->addItem(key);
			ui.listWidget->addItem(value.toString());
		}
		if (key == "avgDayPrice") {
			QJsonValue value = jsonObject.value(key);
			double valueDouble = value.toDouble();
			QString valueStr = QString::number(valueDouble, 'f', 0);
			qDebug() << key << ":" << valueStr;
			ui.listWidget->addItem(key);
			ui.listWidget->addItem(valueStr);
		}
	}
	reply->deleteLater(); //用户有责任在适当的时候删除 QNetworkReply 对象
}

void Enquiry::on_lineEdit_returnPressed() {
	QString url = "https://mp.soulofcinder.xyz/eft/api/mainSearch?tag=&page_num=1&page_size=1&item_name=";
	url = url + ui.lineEdit->text();
	req.setUrl(QUrl(url));
	reply = networkManager->get(req);  //HTML请求

	connect(reply, &QNetworkReply::finished, this, &Enquiry::replyFinished);  //当请求完成时，执行槽函数
}