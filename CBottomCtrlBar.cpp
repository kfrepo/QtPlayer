#include "CBottomCtrlBar.h"
#include <QHBoxLayout>

CBottomCtrlBar::CBottomCtrlBar(QWidget* p)
	:QWidget(p)
{
	//禁止父窗口影响子窗口样式
	setAttribute(Qt::WA_StyledBackground);
	setMouseTracking(true);

	this->setFixedHeight(80);

	setStyleSheet("background-color:rgb(67,67,67)");

	QString label_qss = "QLabel{font-family:Microsoft YaHei; font-size:18px; color:rgb(255,255,255);}";

	m_pCurPlayTimeLabel = new QLabel(this);
	m_pCurPlayTimeLabel->setText("00:00:00");
	m_pCurPlayTimeLabel->setAlignment(Qt::AlignRight);
	m_pCurPlayTimeLabel->setFixedSize(80, 32);
	m_pCurPlayTimeLabel->setStyleSheet(label_qss);

	m_pTotalTimeLabel = new QLabel(this);
	m_pTotalTimeLabel->setText("99:99:99");
	m_pTotalTimeLabel->setAlignment(Qt::AlignLeft);
	m_pTotalTimeLabel->setFixedSize(160, 32);
	m_pTotalTimeLabel->setStyleSheet(label_qss);

	m_pPlayButtonGroupWidget = new CPlayButtonGroupWidget(this);

	m_pToolBoxButton = new QPushButton(this);
	m_pToolBoxButton->setText("");
	m_pToolBoxButton->setFixedSize(32, 32);
	m_pToolBoxButton->setStyleSheet("QPushButton{background-image:url(:/playCtrlBar/resources/playctrlBar/toolbox.svg);border:none;}"
		"QPushButton:hover{background-image:url(:/playCtrlBar/resources/playctrlBar/toolbox_hover.svg);border:none;}"
		"QPushButton:pressed{background-image:url(:/playCtrlBar/resources/playctrlBar/toolbox.svg);border:none;}");

	m_pEffectButton = new QPushButton(this);
	m_pEffectButton->setText("");
	m_pEffectButton->setFixedSize(32, 32);
	m_pEffectButton->setStyleSheet("QPushButton{background-image:url(:/playCtrlBar/resources/playctrlBar/effect.svg);border:none;}"
		"QPushButton:hover{background-image:url(:/playCtrlBar/resources/playctrlBar/effect_hover.svg);border:none;}"
		"QPushButton:pressed{background-image:url(:/playCtrlBar/resources/playctrlBar/effect.svg);border:none;}");

	m_pFullscreenButton = new QPushButton(this);
	m_pFullscreenButton->setText("");
	m_pFullscreenButton->setFixedSize(32, 32);
	m_pFullscreenButton->setStyleSheet("QPushButton{background-image:url(:/playCtrlBar/resources/playctrlBar/fullscreen.svg);border:none;}"
		"QPushButton:hover{background-image:url(:/playCtrlBar/resources/playctrlBar/fullscreen_hover.svg);border:none;}"
		"QPushButton:pressed{background-image:url(:/playCtrlBar/resources/playctrlBar/fullscreen.svg);border:none;}");

	QHBoxLayout* pHLay = new QHBoxLayout(this);
	pHLay->addSpacing(3);
	pHLay->addWidget(m_pCurPlayTimeLabel);
	pHLay->addSpacing(1);
	pHLay->addWidget(m_pTotalTimeLabel);
	pHLay->addStretch();
	pHLay->addWidget(m_pPlayButtonGroupWidget);
	pHLay->addStretch();
	pHLay->addWidget(m_pToolBoxButton);
	pHLay->addSpacing(18);
	pHLay->addWidget(m_pEffectButton);
	pHLay->addSpacing(18);
	pHLay->addWidget(m_pFullscreenButton);

	showTimeLabel(true);

	connect(m_pPlayButtonGroupWidget, &CPlayButtonGroupWidget::sig_play, this, &CBottomCtrlBar::sig_play);
	connect(m_pPlayButtonGroupWidget, &CPlayButtonGroupWidget::sig_stop, this, &CBottomCtrlBar::sig_stop);
	connect(m_pFullscreenButton, &QPushButton::clicked, this, &CBottomCtrlBar::sig_fullScreen);
}

CBottomCtrlBar::~CBottomCtrlBar()
{

}

void CBottomCtrlBar::showTimeLabel(bool isShow)
{
	if (isShow)
	{
		m_pCurPlayTimeLabel->show();
		m_pTotalTimeLabel->show();
	}
	else
	{
		m_pCurPlayTimeLabel->hide();
		m_pTotalTimeLabel->hide();
	}
}

QString CBottomCtrlBar::formatTimeMs(const qint64& timeMs)
{
	qint64 seconds = timeMs / 1000;
	int hours = seconds / 3600;
	int mins = (seconds - hours * 3600) / 60;
	int secs = seconds - hours * 3600 - mins * 60;

	char buf[1024] = { 0 };
	if (hours < 100)
	{
		sprintf(buf, "%02d:%02d:%02d", hours, mins, secs);
	}
	else
	{
		sprintf(buf, "%d:%02d:%02d", hours, mins, secs);
	}

	return QString::fromUtf8(buf);
}

//设置当前播放时间
void CBottomCtrlBar::setCurPlayTime(const qint64& curMs)
{
	m_pCurPlayTimeLabel->setText(formatTimeMs(curMs));
}

//设置总的播放时间
void CBottomCtrlBar::setTotalTime(const qint64& totalMs)
{
	m_pTotalTimeLabel->setText("/" + formatTimeMs(totalMs));
}

//将播放按钮组小部件移动到窗口的中心位置
void CBottomCtrlBar::resizeEvent(QResizeEvent* event)
{
	if (m_pPlayButtonGroupWidget)
	{
		int x = this->width() / 2 - m_pPlayButtonGroupWidget->width() / 2;
		int y = this->height() / 2 - m_pPlayButtonGroupWidget->height() / 2;
		m_pPlayButtonGroupWidget->move(x, y);
	}
}

