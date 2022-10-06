#include "CRightPlayListWidget.h"

CRightPlayListWidget::CRightPlayListWidget(QWidget* p) : QWidget(p)
{
	//禁止父窗口影响子窗口样式
	setAttribute(Qt::WA_StyledBackground);

	this->setFixedWidth(400);
}

CRightPlayListWidget::~CRightPlayListWidget()
{
}