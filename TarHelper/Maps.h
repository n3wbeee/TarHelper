#include <QWidget>
#include "qstring.h"
#include "ui_Maps.h"
#include "qpropertyanimation.h"

class Maps : public QWidget
{
	Q_OBJECT

public:
	Maps(QWidget *parent = Q_NULLPTR);
	~Maps();

	void paintEvent(QPaintEvent *event); 
	bool eventFilter(QObject* watched, QEvent* event);

private slots:
	void on_reserve_clicked();
	void on_forest_clicked();
	void on_interchange_clicked();
	void on_shoreline_clicked();

private:
	Ui::Maps ui;
	int imgSizeDivisor;
	QImage qimg;
	QPropertyAnimation animation;

	void anim_slide(int num);
};