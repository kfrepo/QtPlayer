#pragma once
#include <string>
using namespace std;

string logo_button_qss = R"(
	QPushButton{
		background-color:rgb(54,54,54);
		border:none;
		font-family:Microsoft YaHei;
		font-size:18px; 
		color:rgb(255,255,255);
		padding-right: 2px; /* 调整图片与按钮文字之间的距离 */
	}

	QPushButton::menu-indicator:open{
		image:url(:/titlebar/resources/titleBar/down_arrow.svg);
		subcontrol-position:right center;
subcontrol-origin: content; /* 将指示器相对于按钮内容的边缘定位 */
		subcontrol-origin:padding;border:none;
	}

	QPushButton::menu-indicator:closed{
		image:url(:/titlebar/resources/titleBar/up_arrow.svg);
		subcontrol-position:right center;
		subcontrol-origin:padding;border:none;
	})";

//logo按钮菜单样式
string menu_qss = R"(
	QMenu{background-color:rgb(74,74,74);}
	QMenu::item{
		border: none;
		font:16px;
		color:white;
		background-color:rgb(73, 73, 73);
		padding:8px 32px; /*内边距*/
		margin:0px 0px;
		/* border-bottom:1px solid #DBDBDB; 菜单项的底部边框为1像素宽*/}
	QMenu::item:selected{background-color:rgb(0, 150, 255);}
)";

string minimode_qss = R"(
	QPushButton{
		background-image:url(:/titlebar/resources/titleBar/minimode.svg);
		border:none
	}

	QPushButton:hover{
		background-color:rgb(99, 99, 99);
		background-image:url(:/titlebar/resources/titleBar/minimode_hover.svg);
		border:none;
	}
)";

string settop_qss = R"(
	QPushButton{
		background-image:url(:/titlebar/resources/titleBar/settop.svg);
		border:none
	}

	QPushButton:hover{
		background-color:rgb(99, 99, 99);
		background-image:url(:/titlebar/resources/titleBar/settop_hover.svg);border:none;
	}
)";

string min_qss = R"(
	QPushButton{background-image:url(:/titlebar/resources/titleBar/min.svg);border:none}
		QPushButton:hover{
		background-color:rgb(99, 99, 99);
		background-image:url(:/titlebar/resources/titleBar/min_hover.svg);border:none;}
	)";

string max_qss = R"(
	QPushButton{background-image:url(:/titlebar/resources/titleBar/normal.svg);border:none}
				 QPushButton:hover{
				 background-color:rgb(99, 99, 99);
				 background-image:url(:/titlebar/resources/titleBar/normal_hover.svg);border:none;}
	)";

string close_qss = R"(
	QPushButton{background-image:url(:/titlebar/resources/titleBar/close.svg);border:none}
		QPushButton:hover{
		background-color:rgb(99, 99, 99);
		background-image:url(:/titlebar/resources/titleBar/close_hover.svg);border:none;}
	)";
