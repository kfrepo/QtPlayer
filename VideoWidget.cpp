#include "VideoWidget.h"
#include <QPainter>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMenu>
#include <QDebug>
#include <qt_windows.h>
#include <QApplication>

VideoWidget::VideoWidget(QWidget* parent) : QWidget(parent)
{

	setAttribute(Qt::WA_StyledBackground);  // 禁止父窗口样式影响子控件样式
	this->setMinimumSize(900, 450);

	// 设置背景图片 :/QtPlayer/resources/main_frame_background_static.png
	//this->setStyleSheet("background-image: url(:/QtPlayer/resources/main_frame_background_static.png);"
	//	"background-repeat: no-repeat;");


	this->setAttribute(Qt::WA_OpaquePaintEvent);

	//qApp 是一个全局指针，指向当前正在运行的 Qt 应用程序对象。
	qApp->installNativeEventFilter(this);

	setMouseTracking(true);

	//设置渐变色
	this->setStyleSheet("QWidget{\
                       background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 #09115C, stop:1 #000000 );\
                        font-family:Microsoft YaHei\
                         }");

	QString openfile_button_qss = "QPushButton::menu-indicator:open{"
		"image:url(:/titleBar/resources/titleBar/down_arrow.svg);"
		"subcontrol-position:right center;"
		"subcontrol-origin:padding;border:none;}"
		"QPushButton::menu-indicator:closed{"
		"image:url(:/titleBar/resources/titleBar/up_arrow.svg);"
		"subcontrol-position:right center;"
		"subcontrol-origin:padding;border:none;}";

	m_pTopWidget = new CVideoWidgetTopWidget(this);

	connect(m_pTopWidget, &CVideoWidgetTopWidget::sig_OpenFile, this, &VideoWidget::sig_OpenFile);
	connect(m_pTopWidget, &CVideoWidgetTopWidget::sig_OpenPlaylist, this, &VideoWidget::sig_OpenPlaylist);
	connect(m_pTopWidget, &CVideoWidgetTopWidget::sig_SliderMove, this, &VideoWidget::onSliderMoved);

	connect(this, &VideoWidget::sig_VideoPositionChanged, m_pTopWidget, &CVideoWidgetTopWidget::handleVideoPositionChanged);
}

VideoWidget::~VideoWidget()
{
}

void VideoWidget::onSliderMoved(int value)
{
	//最终调用vlc接口 设置进度重新播放
	emit sig_SliderMoved(value);
}

void VideoWidget::showTopWidget(bool show)
{
	if (show)
	{
		m_pTopWidget->show();
		m_pTopWidget->showOpenBtn(false);
		m_pTopWidget->showSlider(true);
	}
	else
	{
		m_pTopWidget->hide();
	}
}

void VideoWidget::setPlayStatus(bool play)
{
	m_isPlay = play;
}

void VideoWidget::resizeEvent(QResizeEvent* event)
{
	m_dPos = this->pos();

	QPoint pos = this->mapToGlobal(QPoint(0, 0));
	m_pTopWidget->resize(this->width(), this->height());
	m_pTopWidget->move(pos);
	m_pTopWidget->show();
}

//控制选择文件按钮跟随主窗口移动
bool VideoWidget::nativeEventFilter(const QByteArray& eventType, void* message, long* result)
{
	if (eventType == "windows_generic_MSG" || eventType == "windows_dispatcher_MSG") {
		MSG* pMsg = reinterpret_cast<MSG*>(message);
		
		if (pMsg->message == 1025) {
			qDebug() << "VideoWidget msg id = " << pMsg->message << endl;
			QPoint pos = this->window()->mapToGlobal(QPoint(0, 0));

			//移动顶层控制窗口
			m_pTopWidget->move(pos + m_dPos);
		}
	}

	return false;
}