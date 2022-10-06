#pragma once
#include <QWidget>
#include <QSlider>
#include <QAbstractNativeEventFilter>

#include "CVideoWidgetTopWidget.h"

class VideoWidget : public QWidget, public QAbstractNativeEventFilter
{
	Q_OBJECT
public:
	VideoWidget(QWidget* parent = nullptr);
	~VideoWidget();

	void showTopWidget(bool show);
	void setPlayStatus(bool play);

protected:
	//过滤原生事件。你可以监听特定的事件类型，比如窗口消息或者系统消息，然后根据这些消息进行相应的处理。
	bool nativeEventFilter(const QByteArray& eventType, void* message, long* result) override;
	void resizeEvent(QResizeEvent* event) override;
	//当鼠标进入一个窗口部件（widget）时，这个方法会被调用。
	// 你可以在这个方法中编写代码来响应鼠标进入事件，比如改变部件的样式或者显示相关信息。
	/*void enterEvent(QEvent* event) override;
	* 当鼠标离开一个窗口部件（widget）时，这个方法会被调用。你可以在这个方法中编写代码来响应鼠标离开事件，比如恢复部件的原始样式或者隐藏相关信息。
	void leaveEvent(QEvent* event) override;*/

private slots:
	void onSliderMoved(int value);

signals:
	void sig_OpenFile(const QStringList& fileList);
	void sig_OpenFloder(QString path);
	void sig_OpenPlaylist();
	void sig_Totalms(const qint64& duration);
	void sig_VideoPositionChanged(const qint64& pos);
	void sig_SliderMoved(int value);

private:
	QPoint m_dPos;   //VideoWidget相对于父窗口的位置
	CVideoWidgetTopWidget* m_pTopWidget = nullptr;
	bool m_isPlay = false;
};

