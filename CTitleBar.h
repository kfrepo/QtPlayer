#pragma once
#include <QWidget>
#include <QLabel>
#include <QPushButton>

/**
* 顶部标题栏
*/
class CTitleBar : public QWidget
{
	Q_OBJECT

public:
	CTitleBar(QWidget* p = nullptr);
	~CTitleBar();

private:
	void initUI();

private:
	void mousePressEvent(QMouseEvent* event) override;
	void mouseDoubleClickEvent(QMouseEvent* event) override;

private slots:
	void onClicked();
	void slot_openfile();

signals:
	void sig_close();
	void sig_openfile(const QStringList& fileList);

private:
	QPushButton* m_pLogoBtn;

	QPushButton* m_pMinimodeBtn;
	QPushButton* m_pSettopBtn;
	QPushButton* m_pMinBtn;
	QPushButton* m_pMaxBtn;
	QPushButton* m_pCloseBtn;
};

