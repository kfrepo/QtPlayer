#include "CTitleBar.h"
#include <QHBoxLayout>
#include "qss.h"
#include <QMenu>
#include <QAction>

#include <QFileDialog>
#include <QSettings>
#include <QStandardPaths>
#include <QDebug>

#pragma comment(lib, "user32.lib")
#include <qt_windows.h>

CTitleBar::CTitleBar(QWidget* p) :
	QWidget(p)
{
	this->setAttribute(Qt::WA_DeleteOnClose);
	setMouseTracking(true);// 设置窗体追踪鼠标
	initUI();
}

CTitleBar::~CTitleBar()
{
}

void CTitleBar::initUI()
{
	//禁止父窗口影响子窗口样式
	setAttribute(Qt::WA_StyledBackground);
	this->setFixedHeight(35 + 5 * 2);
	this->setStyleSheet("background-color:rgb(54,54,54)");

	m_pLogoBtn = new QPushButton(this);
	m_pLogoBtn->setFixedSize(100, 32);
	m_pLogoBtn->setText("QT影音");
	m_pLogoBtn->setStyleSheet(QString::fromStdString(logo_button_qss));


	//菜单
	QMenu* pMenu = new QMenu(this);
	pMenu->setStyleSheet(QString::fromStdString(menu_qss));

	//1、文件菜单
	QMenu* fileMenuItems = new QMenu;
	fileMenuItems->setTitle("文件");

	QList<QAction*> acList;
	QAction* openFileAc = new QAction("打开文件", this);
	QAction* openFloderAc = new QAction("打开文件夹", this);
	QAction* openUrlAc = new QAction("打开url", this);
	acList << openFileAc << openFloderAc << openUrlAc;
	fileMenuItems->addActions(acList);
	fileMenuItems->setStyleSheet(QString::fromStdString(menu_qss));

	QAction* pAc2 = new QAction("播放", this);
	QAction* pAc3 = new QAction("工具", this);
	QAction* pAc4 = new QAction("退出", this);

	pMenu->addMenu(fileMenuItems);
	pMenu->addAction(pAc2);
	pMenu->addAction(pAc3);
	pMenu->addAction(pAc4);

	m_pLogoBtn->setMenu(pMenu);

	m_pMinimodeBtn = new QPushButton(this);
	m_pMinimodeBtn->setFixedSize(32, 32);
	m_pMinimodeBtn->setStyleSheet(QString::fromStdString(minimode_qss));

	m_pSettopBtn = new QPushButton(this);
	m_pSettopBtn->setFixedSize(32, 32);
	m_pSettopBtn->setStyleSheet(QString::fromStdString(settop_qss));

	m_pMinBtn = new QPushButton(this);
	m_pMinBtn->setFixedSize(32, 32);
	m_pMinBtn->setStyleSheet(QString::fromStdString(min_qss));

	m_pMaxBtn = new QPushButton(this);
	m_pMaxBtn->setFixedSize(32, 32);
	m_pMaxBtn->setStyleSheet(QString::fromStdString(max_qss));

	m_pCloseBtn = new QPushButton(this);
	m_pCloseBtn->setFixedSize(32, 32);
	m_pCloseBtn->setStyleSheet(QString::fromStdString(close_qss));

	QHBoxLayout* pHlay = new QHBoxLayout(this);
	pHlay->addWidget(m_pLogoBtn);

	pHlay->addStretch();
	pHlay->addWidget(m_pMinimodeBtn);
	QSpacerItem* pItem = new QSpacerItem(20, 20, QSizePolicy::Fixed, QSizePolicy::Fixed);
	pHlay->addSpacerItem(pItem);

	pHlay->addWidget(m_pSettopBtn);
	QSpacerItem* pItem1 = new QSpacerItem(20, 20, QSizePolicy::Fixed, QSizePolicy::Fixed);
	pHlay->addSpacerItem(pItem1);

	pHlay->addWidget(m_pMinBtn);
	QSpacerItem* pItem2 = new QSpacerItem(18, 20, QSizePolicy::Fixed, QSizePolicy::Fixed);
	pHlay->addSpacerItem(pItem2);
	pHlay->addWidget(m_pMaxBtn);

	QSpacerItem* pItem3 = new QSpacerItem(18, 20, QSizePolicy::Fixed, QSizePolicy::Fixed);
	pHlay->addSpacerItem(pItem3);   // 弹簧每次使用时得new出来，不能重复使用
	pHlay->addWidget(m_pCloseBtn);

	//左、上、右、下
	pHlay->setContentsMargins(10, 6, 10, 6);

	connect(openFileAc, &QAction::triggered, this, &CTitleBar::slot_openfile);
	connect(m_pMinBtn, &QPushButton::clicked, this, &CTitleBar::onClicked);
	connect(m_pMaxBtn, &QPushButton::clicked, this, &CTitleBar::onClicked);
	connect(m_pCloseBtn, &QPushButton::clicked, this, &CTitleBar::onClicked);
}

//实现自定义标题栏的鼠标按下事件处理函数 mousePressEvent。
// 它的作用是实现窗口的拖动，允许用户通过鼠标点击标题栏并拖动来移动窗口
void CTitleBar::mousePressEvent(QMouseEvent* event)
{
	if (ReleaseCapture())
	{
		QWidget* pWindow = this->window();
		if (pWindow->isTopLevel())
		{
			SendMessage(HWND(pWindow->winId()), WM_SYSCOMMAND, SC_MOVE + HTCAPTION, 0);
		}
	}
}

void CTitleBar::mouseDoubleClickEvent(QMouseEvent* event)
{
	emit m_pMaxBtn->clicked();
}

void CTitleBar::onClicked()
{
	QPushButton* pButton = qobject_cast<QPushButton*>(sender());

	QWidget* pWindow = this->window();

	if (pButton == m_pMinBtn)
	{
		pWindow->showMinimized();
	}
	else if (pButton == m_pMaxBtn)
	{
		if (pWindow->isMaximized())
		{
			pWindow->showNormal();
			m_pMaxBtn->setStyleSheet("QPushButton{background-image:url(:/titlebar/resources/titleBar/normal.svg);border:none}" \
				"QPushButton:hover{" \
				"background-color:rgb(99, 99, 99);" \
				"background-image:url(:/titlebar/resources/titleBar/normal_hover.svg);border:none;}");
		}
		else
		{
			pWindow->showMaximized();
			m_pMaxBtn->setStyleSheet("QPushButton{background-image:url(:/titlebar/resources/titleBar/max.svg);border:none}" \
				"QPushButton:hover{" \
				"background-color:rgb(99, 99, 99);" \
				"background-image:url(:/titlebar/resources/titleBar/max_hover.svg);border:none;}");
		}
	}
	else if (pButton == m_pCloseBtn)
	{
		emit sig_close();
	}
}

void CTitleBar::slot_openfile() {

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

	qDebug() << "TitleBar Selected files:";
	for (const QString& filePath : filelist) {
		qDebug() << filePath;
	}

	int end = filelist[0].lastIndexOf("/");
	QString tmppath = filelist[0].left(end + 1);
	settings.setValue("openfile_path", tmppath);  // 将当前打开的路径写入到注册表

	emit sig_openfile(filelist);
}


