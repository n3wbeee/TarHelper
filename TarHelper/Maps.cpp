#include "maps.h"
#include "qdir.h"
#include "qimage.h"
#include "qdebug.h"
#include "qpoint.h"
#include "qevent.h"
#include "qthread.h"
#include "qscrollbar.h"
#include "qgraphicsscene.h"
#include "qdesktopservices.h"

Maps::Maps(QWidget *parent)
	: QWidget(parent), qimg(":/Maps/reserve"), 
	imgSizeDivisor(1), fileName("reserve.jpg") {	
	ui.setupUi(this);

	ui.mapArea->installEventFilter(this);	//安装事件过滤器

	/*QGraphicsScene初始化与载入图片*/
	scene = new QGraphicsScene(this);
	ui.mapArea->setScene(scene);
	qimg.load(":/Maps/reserve");
	scene->addPixmap(QPixmap::fromImage(qimg));

	animation.setDuration(250);
	animation.setTargetObject(ui.slider);
	animation.setEasingCurve(QEasingCurve::InOutCirc);
}

Maps::~Maps() {
	delete scene;
}

void Maps::paintEvent(QPaintEvent *event) {
	QWidget::paintEvent(event);
	static int paint(0);	//重载paintEvent事件，第一次载入窗口时将滑块置初始位置
	if (paint <= 2) {		//窗口每次重绘都会调用paintEvent，必须设置个阈值不然动画效果无效
		ui.slider->move(0, 0);
		++paint;
	}
}

/*图片选择*/
void Maps::on_reserve_clicked() {
	anim_slide(0);
	qimg.load(":/Maps/reserve");
	fileName = "reserve.jpg";
	scene->clear();
	scene->setSceneRect(0, 0, qimg.width(), qimg.height());
	scene->addPixmap(QPixmap::fromImage(qimg));
}

void Maps::on_forest_clicked() {
	anim_slide(3);
	qimg.load(":/Maps/forest");
	fileName = "forest.png";
	scene->clear();
	scene->setSceneRect(0, 0, qimg.width(), qimg.height());
	scene->addPixmap(QPixmap::fromImage(qimg));
}

void Maps::on_interchange_clicked() {
	anim_slide(1);
	qimg.load(":/Maps/interchange");
	fileName = "interchange.jpg";
	scene->clear();
	scene->setSceneRect(0, 0, qimg.width(), qimg.height());
	scene->addPixmap(QPixmap::fromImage(qimg));
}

void Maps::on_shoreline_clicked() {
	anim_slide(2);
	qimg.load(":/Maps/shoreline");
	fileName = "shoreline.jpg";
	scene->clear();
	scene->setSceneRect(0, 0, qimg.width(), qimg.height());
	scene->addPixmap(QPixmap::fromImage(qimg));
}

void Maps::on_reset_clicked() {
	/*如果没有文件夹就创建一个*/
	if (!QFile::exists("./maps")) {
		QDir qdir;
		qdir.mkdir("./maps");
	}
	/*更新图片文件*/
	QFile::remove("./maps/forest.png");
	QFile::copy(":/Maps/forest", "./maps/forest.png");
	QFile::remove("./maps/interchange.jpg");
	QFile::copy(":/Maps/interchange", "./maps/interchange.jpg");
	QFile::remove("./maps/reserve.jpg");
	QFile::copy(":/Maps/reserve", "./maps/reserve.jpg");
	QFile::remove("./maps/shoreline.jpg");
	QFile::copy(":/Maps/shoreline", "./maps/shoreline.jpg");
}

void Maps::on_openOutside_clicked() {
	QString filePath = "./Maps/" + fileName;  //获得当前正在显示的图像是什么
	QDesktopServices::openUrl(	//打开文件
		QUrl::fromLocalFile(filePath));
}

void Maps::anim_slide(int num) {
	animation.setPropertyName("pos");
	animation.setStartValue(ui.slider->pos());
	animation.setEndValue(QPoint(0, 48 * num));
	animation.start();
}

void QGraphicsView::wheelEvent(QWheelEvent* event) {
	return;		//重载滚轮事件 让其忽略滚轮
}

bool Maps::eventFilter(QObject* watched, QEvent* event) {
	if (watched == ui.mapArea && event->type() == QEvent::Wheel) {
		QWheelEvent* wheelEvent = static_cast<QWheelEvent*>(event);
		if (wheelEvent->delta() < 0) {
			ui.mapArea->scale(5.0 / 6, 5.0 / 6);
		}
		else {
			ui.mapArea->scale(6.0 / 5, 6.0 / 5);
		}
	}
	return false;
}