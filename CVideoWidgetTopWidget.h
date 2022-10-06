#pragma once
#include <QWidget>
#include <QPushButton>
#include "COpenFileButton.h"
#include <QSlider>
#include "CTimeSlider.h"

class CVideoWidgetTopWidget : public QWidget
{
	Q_OBJECT

public:
	CVideoWidgetTopWidget(QWidget* p = nullptr);
	~CVideoWidgetTopWidget();

	void setTimeSliderRange(qint64 value);
	void setSliderValue(qint64 value);
	void showRightBtn(bool flag);
	void showOpenBtn(bool flag);
	void showSlider(bool flag);
	void setPlayStatus(bool play) { m_isPlay = play; }
protected:
	void resizeEvent(QResizeEvent* event) override;

public slots:
	void onSliderMoved(int position);
	void handleVideoPositionChanged(const qint64& pos);

signals:
	void sig_OpenFile(const QStringList& fileList);
	void sig_OpenFloder(QString path);
	void sig_OpenPlaylist();
	void sig_SliderMove(int value);

private:
	QPushButton* m_pOpenrRightlistButton = nullptr;
	COpenFileButton* m_pOpenFileButton = nullptr;
	CTimeSlider* m_pTimeSlider = nullptr;

	int m_bSliderPressed = false;
	bool m_isPlay = false;
};

