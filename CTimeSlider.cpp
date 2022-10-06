#include "CTimeSlider.h"
#include <QEvent>
#include <QMouseEvent>

CTimeSlider::CTimeSlider(QWidget* p)
	:QSlider(p)
{
	setAttribute(Qt::WA_StyledBackground);
	 
	this->setOrientation(Qt::Horizontal);
	this->setFixedHeight(m_SliderHeight);
	this->setRange(0, 99);
	this->setSingleStep(0);
	this->setPageStep(0);

	// 给事件滑动条安装事件，处理点击效果
	this->installEventFilter(this);

	QString slider_qss = "QSlider{background:transparent; \
		border-style: outset; \
		border-style: outset;  \
			border-radius: 10px;} \
		QSlider::groove:horizontal{ \
			height: 20px; \
			background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #B1B1B1, stop:1 #c4c4c4); \
			margin: 2px 0} \
		QSlider::handle:horizontal { \
			background: QRadialGradient(cx:0, cy:0, radius: 1, fx:0.5, fy:0.5, stop:0 white, stop:1 green); \
			width: 16px; \
			height: 16px; \
			margin: -5px 6px -5px 6px; \
			border-radius:11px;  \
		border: 3px solid #ffffff;}";

	//setStyleSheet(slider_qss);


	this->setStyleSheet(
		"QSlider{background:transparent;"
		"border-style: outset;}"
		"QSlider::groove:horizontal {"
		"    border: 1px solid transparent;"
		"    height: 10px;"
		"    background: #B0C4DE;"
		"    margin: 0px 0;"
		"}"
		//"QSlider::add-page:horizontal { background-color: #0096ff; width:5px; border-radius: 2px; }"
		"QSlider::sub-page:horizontal { background-color: #0096ff; width:5px; border-radius: 2px; }"
		"QSlider::handle:horizontal {"
		"    background: qradialgradient(spread:pad, cx:0.5, cy:0.5, radius:0.5, fx:0.5, fy:0.5,"
		"                                  stop:0 #3266ed, stop:1 #3266ed);"
		"    width: 20px;"
		"    height: 20px;"
		"    margin: -5px -10px -5px -10px;"
		"    border-radius: 10px;"
		"}"
	);


	connect(this, &QSlider::sliderMoved, this, &CTimeSlider::onSliderMoved);
}

CTimeSlider::~CTimeSlider()
{
}

bool CTimeSlider::eventFilter(QObject* watched, QEvent* event)
{
	if (event->type() == QEvent::MouseButtonPress && watched == this)
	{
		slider_mouseLButtonPress(watched, event);
		//return true; // 表示事件已经处理
	}

	return QWidget::eventFilter(watched, event);
}

//处理鼠标左键按下事件
void CTimeSlider::slider_mouseLButtonPress(QObject* slider, QEvent* event)
{
	do
	{
		//只处理 鼠标左键 的按下事件
		QSlider* sliderCtrl = static_cast<QSlider*>(slider);
		QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
		if (Qt::LeftButton != mouseEvent->button())
		{
			break;
		}

		// 确定控件操作的基本参数
		int cxctl = 0;  // 滑块宽度
		int cxwnd = 0;  // 滑槽长度
		int mxpos = 0;  // 鼠标按下的位置

		if (Qt::Horizontal == sliderCtrl->orientation())
		{
			// 水平样式的 slider
			cxctl = sliderCtrl->minimumSizeHint().width();
			cxwnd = sliderCtrl->width();
			//外观是否被反转
			if (sliderCtrl->invertedAppearance()) {
				mxpos = cxwnd - mouseEvent->x();
			}
			else {
				mxpos = mouseEvent->x();
			}
			qDebug() << "CTimeSlider cxctl:" << cxctl;
			qDebug() << "CTimeSlider cxwnd:" << cxwnd;
			qDebug() << "CTimeSlider mxpos:" << mxpos;
		}
		else
		{
			// 垂直样式的 slider
			cxctl = sliderCtrl->minimumSizeHint().height();
			cxwnd = sliderCtrl->height();
			if (sliderCtrl->invertedAppearance())
				mxpos = mouseEvent->y();
			else
				mxpos = cxwnd - mouseEvent->y();
		}

		//滑槽小于等于滑块
		if (cxwnd <= cxctl)
		{
			break;
		}

		// 计算结果，并设置新计算得到的 position 值   鼠标/(滑槽-滑块)
		//int value = minimum() + ((maximum() - minimum()) * event->x()) / width();
		int scpos = sliderCtrl->minimum() +
			(int)((sliderCtrl->maximum() - sliderCtrl->minimum()) *
				((mxpos - cxctl / 2.0) / (cxwnd - cxctl)));
		qDebug() << "CTimeSlider sliderCtrl->maximum():" << sliderCtrl->maximum();
		qDebug() << "CTimeSlider sliderCtrl->minimum():" << sliderCtrl->minimum();

		qDebug() << "CTimeSlider setSliderPosition:" << scpos;
		if (sliderCtrl->sliderPosition() == scpos)
		{
			break;
		}

		//这个位置的值应该在滑块的最小值和最大值之间
		sliderCtrl->setSliderPosition(scpos);

		//停止播放 跳转  恢复播放
		emit sig_SliderMove(scpos);
	} while (0);
}

void CTimeSlider::onSliderMoved(int position)
{
	qDebug() << "CTimeSlider move send sig:" << position;
	emit sig_SliderMove(position);
}

void CTimeSlider::setTimeSliderRange(qint64 value)
{
	this->setRange(0, value);
}

void CTimeSlider::setSliderValue(qint64 value)
{
	this->setValue(value);
}