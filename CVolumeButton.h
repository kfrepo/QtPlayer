/**
* 音量调节按钮
* 功能：
	1. 鼠标悬浮到音量时显示slider_dialog
	2. 点击时mute
* 注意问题：重写按钮类，样式表无效
* 
*/
#pragma once

#include <QPushButton>
#include "CVolumeSliderDialog.h"

class CVolumeButton : public QPushButton
{
	Q_OBJECT

public:
	CVolumeButton(QWidget* parent);
	~CVolumeButton();

	bool getMute() const
	{
		return m_isMute;
	}

	void setMute(bool mute) { m_isMute = mute; }

signals:
	void sig_VolumeValue(int value);

protected:
	void paintEvent(QPaintEvent* event) override;

	//鼠标进入 弹出音量窗口
	void enterEvent(QEvent* event) override;
	//void leaveEvent(QEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;
	void timerEvent(QTimerEvent* event) override;

private:
	bool m_isMute = false;  //是否静音
	CVolumeSliderDialog* m_pVolumeSliderDlg = nullptr;
};
