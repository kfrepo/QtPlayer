#include "COpenFileButton.h"
#include <QHBoxLayout>
#include <QPainter>
#include <QMenu>
#include <QEvent>
#include <QFileDialog>
#include <QSettings>
#include <QStandardPaths>
#include <QDebug>

#define LEFT_BUTTON_WIDTH 145
#define RIGHT_BUTTON_WIDTH 60
#define BUTTON_HEIGHT 40

COpenFileButton::COpenFileButton(QWidget* p) : QWidget(p)
{
	setAttribute(Qt::WA_StyledBackground);  // 禁止父窗口样式影响子控件样式

	//设置总宽度
	m_totalWidth = LEFT_BUTTON_WIDTH + RIGHT_BUTTON_WIDTH;

	m_pOpenFileButton = new QPushButton(this);
	m_pOpenFileButton->setText("打开文件");
	m_pOpenFileButton->setFixedSize(LEFT_BUTTON_WIDTH, BUTTON_HEIGHT);

	QString qssLeft = "QPushButton{"
		"padding-left:30px;"
		"border-top-left-radius:20px;border-bottom-left-radius:20px;"
		"background-color: rgb(55, 55, 55);"
		"color:rgb(255, 255, 255);"
		"text-align:left;"
		"}";

	m_pOpenFileButton->setIcon(QIcon(":/videoWidget/resources/videoWidget/openfile.svg"));
	m_pOpenFileButton->setLayoutDirection(Qt::LeftToRight);
	m_pOpenFileButton->setStyleSheet(qssLeft);

	m_pArrowButton = new QPushButton(this);
	m_pArrowButton->setFixedSize(RIGHT_BUTTON_WIDTH, BUTTON_HEIGHT);

	QString qssRight = "QPushButton{"
		"background-image: url(:/videoWidget/resources/videoWidget/vline.svg);"
		"background-repeat: no-repeat;"
		"background-origin: padding;"
		"background-position: left;"
		"border-top-right-radius:20px;border-bottom-right-radius:20px;"
		"background-color: rgb(55, 55, 55);}"
		"QPushButton::menu-indicator:open{"
		"image:url(:/videoWidget/resources/videoWidget/down_arrow_16px.svg);"
		"subcontrol-position:right center;right:10px;"
		"subcontrol-origin:padding;border:none;}"
		"QPushButton::menu-indicator:closed{"
		"image:url(:/videoWidget/resources/videoWidget/down_arrow_16px.svg);"
		"subcontrol-position:right center;right:10px;"
		"subcontrol-origin:padding;border:none;}";
	m_pArrowButton->setStyleSheet(qssRight);

	QString menu_qss = "QMenu{background-color:rgb(253,253,253);}"
		"QMenu::item{"
		"font:16px;"
		"background-color:rgb(253,253,253);"
		"padding:8px 32px;"
		"margin:0px 8px;"
		"border-bottom:1px solid #DBDBDB;}"
		"QMenu::item:selected{background-color: #FFF8DC;}";

	QMenu* pMenu = new QMenu(this);
	pMenu->setStyleSheet(menu_qss);
	pMenu->setFixedWidth(m_totalWidth);
	QAction* pAc1 = new QAction("打开文件", this);
	QAction* pAc2 = new QAction("打开文件夹 ", this);
	pMenu->addAction(pAc1);
	pMenu->addAction(pAc2);
	m_pArrowButton->setMenu(pMenu);
	pMenu->installEventFilter(this);  //设置事件过滤，调整弹出菜单的位置

	QHBoxLayout* pHLay = new QHBoxLayout(this);
	pHLay->addWidget(m_pOpenFileButton);
	pHLay->setSpacing(3);
	pHLay->addWidget(m_pArrowButton);
	pHLay->setContentsMargins(0, 0, 0, 0);

	setLayout(pHLay);

	setFixedSize(m_totalWidth, BUTTON_HEIGHT);
	setWindowFlags(Qt::FramelessWindowHint);
	setAttribute(Qt::WA_TranslucentBackground);  //设置窗口背景透明, 为圆角窗口准备

	connect(m_pOpenFileButton, &QPushButton::clicked, this, &COpenFileButton::on_openfile);
	connect(pAc1, &QAction::triggered, this, &COpenFileButton::on_openfile);
	connect(pAc2, &QAction::triggered, this, &COpenFileButton::on_openFloder);
}

COpenFileButton::~COpenFileButton()
{
}

void COpenFileButton::paintEvent(QPaintEvent* event)
{
	//绘制了一个圆角矩形按钮
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);  // 反锯齿
	painter.setBrush(QBrush(QColor(55, 55, 55)));
	painter.setPen(Qt::transparent);
	QRect rect = this->rect();
	painter.drawRoundedRect(rect, BUTTON_HEIGHT / 2, BUTTON_HEIGHT / 2);  //设置窗口圆角 15px

	QWidget::paintEvent(event);
}

bool COpenFileButton::eventFilter(QObject* watched, QEvent* event)
{
	if (m_pArrowButton)
	{
		if (event->type() == QEvent::Show && watched == m_pArrowButton->menu())
		{
			int menuXPos = m_pArrowButton->menu()->pos().x() - LEFT_BUTTON_WIDTH;   //默认是显示在当前按钮的左下方
			int menuWidth = m_pArrowButton->menu()->size().width();
			int buttonWidth = m_pArrowButton->size().width();
			QPoint pos = QPoint(menuXPos, m_pArrowButton->menu()->pos().y() + 2);

			m_pArrowButton->menu()->move(pos);
			return true;
		}
	}

	return false;
}

void COpenFileButton::on_openfile()
{
	QString cfgPath = "HKEY_CURRENT_USER\\Software\\MediaPlayer";
	QSettings settings(cfgPath, QSettings::NativeFormat);
	QString lastPath = settings.value("openfile_path").toString();  // 从注册表获取路径

	if (lastPath.isEmpty())
	{
		lastPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);  //获取默认的文档路径 
	}

	QString filter = "视频文件(*.flv *.rmvb *.avi *.mp4);;所有文件(*.*)";
	//可以同时打开多个文件
	QStringList filelist = QFileDialog::getOpenFileNames(
		this,
		"选择要播放的文件",
		lastPath,
		filter);

	if (filelist.isEmpty())
	{
		return;
	}

	qDebug() << "Selected files:";
	for (const QString& filePath : filelist) {
		qDebug() << filePath;
	}

	int end = filelist[0].lastIndexOf("/");
	QString tmppath = filelist[0].left(end + 1);
	settings.setValue("openfile_path", tmppath);  // 将当前打开的路径写入到注册表

	emit sig_openfile(filelist);
}

void COpenFileButton::on_openFloder()
{
	QString path = QFileDialog::getExistingDirectory(this, "打开文件夹", "/");

	if (path.isEmpty())
	{
		return;
	}
	qDebug() << "Selected path:" << path;

	emit sig_openFloder(path);
}