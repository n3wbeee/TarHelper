#include "Maps.h"
#include "qimage.h"
#include "qdebug.h"
#include "qpoint.h"
#include "qevent.h"
#include "qthread.h"
#include "qscrollbar.h"
#include "qgraphicsscene.h"

Maps::Maps(QWidget *parent)
	: QWidget(parent), qimg(":/Maps/reserve"), imgSizeDivisor(1)
{	
	ui.setupUi(this);

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
	scene->clear();
	scene->setSceneRect(0, 0, qimg.width(), qimg.height());
	scene->addPixmap(QPixmap::fromImage(qimg));
}

void Maps::on_forest_clicked() {
	anim_slide(3);
	qimg.load(":/Maps/forest");
	scene->clear();
	scene->setSceneRect(0, 0, qimg.width(), qimg.height());
	scene->addPixmap(QPixmap::fromImage(qimg));
}

void Maps::on_interchange_clicked() {
	anim_slide(1);
	qimg.load(":/Maps/interchange");
	scene->clear();
	scene->setSceneRect(0, 0, qimg.width(), qimg.height());
	scene->addPixmap(QPixmap::fromImage(qimg));
}

void Maps::on_shoreline_clicked() {
	anim_slide(2);
	qimg.load(":/Maps/shoreline");
	scene->clear();
	scene->setSceneRect(0, 0, qimg.width(), qimg.height());
	scene->addPixmap(QPixmap::fromImage(qimg));
}

void Maps::anim_slide(int num) {
	animation.setPropertyName("pos");
	animation.setStartValue(ui.slider->pos());
	animation.setEndValue(QPoint(0, 48 * num));
	animation.start();
}