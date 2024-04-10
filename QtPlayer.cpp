#include "QtPlayer.h"
#include "QMessageBox"
#include "QVBoxLayout"
#include "QHBoxLayout"
#include <QKeyEvent>
#include "QDebug"


QtPlayer::QtPlayer(QWidget *parent)
    : CFrameLessWidgetBase(parent)
{
    this->setWindowTitle("vlcmain");
    setMouseTracking(true);

    m_pVlc = std::make_unique<CVlcKits>();

    int nRet = m_pVlc->initVLC();
    switch (nRet)
    {
        case -1:
            QMessageBox::information(this, "提示", "libvlc_new failed");
            exit(EXIT_FAILURE);

        case -2:
            QMessageBox::information(this, "提示", "libvlc_media_player_new failed");
            exit(EXIT_FAILURE);
    }

    initUI();
}

QtPlayer::~QtPlayer()
{
}

void QtPlayer::initUI()
{

    QIcon icon(":/QtPlayer/resources/logo.png");
    // 设置窗口图标
    setWindowIcon(icon);

    m_pTitleBar = new CTitleBar(this);
    m_pVideoWidget = new VideoWidget(this);
    m_pRightPlayListWidget = new CRightPlayListWidget(this);
    m_pRightPlayListWidget->hide();

    m_pBottomCtrlBar = new CBottomCtrlBar(this);

    
    QVBoxLayout* pMainVlay = new QVBoxLayout(this);

    //1、顶部菜单栏
    pMainVlay->addWidget(m_pTitleBar);
    pMainVlay->setSpacing(0);

    //2、视频播放区域
    QHBoxLayout* pHlay = new QHBoxLayout;
    pHlay->addWidget(m_pVideoWidget);
    pHlay->addWidget(m_pRightPlayListWidget);
    //左边、上边、右边、下边
    pHlay->setContentsMargins(0, 0, 0, 0);
    pMainVlay->addLayout(pHlay);

    //3、底部状态栏
    pMainVlay->addWidget(m_pBottomCtrlBar);
    pMainVlay->setContentsMargins(0, 0, 0, 0);

    connect(m_pTitleBar, &CTitleBar::sig_close, this, &QtPlayer::on_closeSlot);
    connect(m_pTitleBar, &CTitleBar::sig_openfile, this, &QtPlayer::onOpenFile);

    connect(m_pVideoWidget, &VideoWidget::sig_OpenFile, this, &QtPlayer::onOpenFile);
    connect(m_pVideoWidget, &VideoWidget::sig_SliderMoved, this, &QtPlayer::onTimeSliderMoved);
    connect(m_pBottomCtrlBar, &CBottomCtrlBar::sig_play, this, &QtPlayer::onPlay);

    //根据 vlc 回调信息修改UI  进度条、时间lable
    connect(m_pVlc.get(), &CVlcKits::sig_UpdateTimeText, [=](const QString& str, const qint64& cur, const qint64& total) {
        m_pVideoWidget->sig_Totalms(total);
        });
    connect(m_pVlc.get(), &CVlcKits::sig_TimeSliderPos, m_pVideoWidget, &VideoWidget::sig_VideoPositionChanged);
    

    connect(m_pVlc.get(), &CVlcKits::sig_UpdateTimeText, [=](const QString& str, const qint64& cur, const qint64& total) {
        qDebug() << "QtPlayer cur:" << cur << " total:" << total;
        m_pBottomCtrlBar->setCurPlayTime(cur);
        m_pBottomCtrlBar->setTotalTime(total);
        });

}


void QtPlayer::onOpenFile(const QStringList& fileList)
{
    if (fileList.isEmpty())
        return;

    if (0 != m_pVlc->play(fileList, (void*)(m_pVideoWidget->winId())))
    {
        QMessageBox::information(this, "提示", "播放失败");
        return;
    }

    //SetTimer 是 Windows API 中用于创建定时器的函数之一。它可以创建一个定时器，并在指定的时间间隔内触发一个定时器消息。
    // 窗口句柄 id  定时时间  TimeProc指向的函数将在定时器触发时被调用
    SetTimer(NULL, 1, 300, TimeProc);
    m_pVideoWidget->setPlayStatus(true);
    m_isPlay = true;
}

void QtPlayer::onTimeSliderMoved(int value)
{
    m_pVlc->setVideoPostion(value);
}

void QtPlayer::onPlay()
{
    m_pVlc->play();
}

void QtPlayer::on_closeSlot()
{
    //QMessageBox::StandardButton _exit = QMessageBox::warning(this, "提示", "确定要退出吗",
    //    QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

    //if (_exit == QMessageBox::Yes)
    //{
    //    close();
    //}
    close();
}

void QtPlayer::mouseDoubleClickEvent(QMouseEvent* event)
{
    if (this->isFullScreen())
    {
        this->showNormal();
    }
    else
    {
        this->showFullScreen();
    }
}

void QtPlayer::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Escape)
    {
        if (this->isFullScreen())
        {
            this->showNormal();
        }
    }
}

void QtPlayer::mouseMoveEvent(QMouseEvent* event)
{
    //qDebug() << "mouseMoveEvent";

    // 获取视频窗口的绝对位置  
    // 获取了视频窗口 m_pVideoWidget 的在屏幕上的绝对位置。mapToGlobal() 函数将窗口内的相对坐标（0,0）转换为屏幕上的绝对坐标。
    QPoint p1 = m_pVideoWidget->mapToGlobal(QPoint(0, 0));
    QRect rect = m_pVideoWidget->rect();

    // 绝对矩形区域 视频窗口在屏幕上的绝对位置，大小和视频窗口相同。
    QRect rect2(p1.x(), p1.y(), rect.width(), rect.height());

    // 鼠标的绝对位置
    QPoint p2 = QCursor::pos();

    if (m_isPlay)
    {
        if (rect2.contains(p2))
        {
            //qDebug() << "in";
            m_pVideoWidget->showTopWidget(true);
        }
        else
        {
            //qDebug() << "out";
            m_pVideoWidget->showTopWidget(false);
        }
    }
}




