#pragma once

#include <QtWidgets/QWidget>
#include "CFrameLessWidgetBase.h"
#include "CTitleBar.h"
#include "VideoWidget.h"
#include "CRightPlayListWidget.h"
#include "CBottomCtrlBar.h"
#include "CVlcKits.h"

#include <Windows.h>
#include <memory>

class QtPlayer : public CFrameLessWidgetBase
{
    Q_OBJECT

public:
    QtPlayer(QWidget *parent = nullptr);
    ~QtPlayer();

private:
    void initUI();
private:
    void mouseDoubleClickEvent(QMouseEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;

    // 处理枚举过程中的每个子窗口。
    static BOOL CALLBACK EnumVLC(HWND hwnd, LPARAM lParam)
    {
        TCHAR szTitle[1024];
        //获取指定窗口的标题，并将其存储在 szTitle 缓冲区中
        int nLen = GetWindowText(hwnd, szTitle, 1024);

        if (nLen > 0)
        {
            // 如果成功获取到窗口标题，就调用 EnableWindow 函数将该窗口禁用，使其不响应用户输入。
            EnableWindow(hwnd, FALSE);
            //调用 KillTimer 函数停止之前创建的定时器
            KillTimer(NULL, 1);
        }
        //返回 TRUE 表示继续枚举下一个子窗口，返回 FALSE 表示停止枚举
        return TRUE;
    }

    // hwnd: 窗口句柄，指示与定时器相关联的窗口句柄。由于在 SetTimer 中传递了 NULL，因此这里的值通常为 NULL。
    // msg: 一个无符号整数，表示定时器消息。在定时器回调函数中，通常不使用这个参数。
    // id : 定时器的ID号，与 SetTimer 中设置的ID号相同。
    // time : 当前系统时间。
    // TimeProc 回调函数的主要功能是在定时器触发时搜索具有类名为 "vlcmain" 的顶层窗口，并枚举该窗口的所有子窗口。
    static void CALLBACK TimeProc(HWND hwnd, UINT msg, UINT_PTR id, DWORD time)
    {
        HWND vlcHwnd = FindWindowEx(NULL, NULL, NULL, L"vlcmain");

        if (vlcHwnd)
        {
            EnumChildWindows(vlcHwnd, EnumVLC, NULL);
        }
    }

private slots:
    void on_closeSlot();
    void onOpenFile(const QStringList& fileList);
    void onTimeSliderMoved(int value);
    void onPlay();

private:
    CTitleBar* m_pTitleBar;
    VideoWidget* m_pVideoWidget;
    CRightPlayListWidget* m_pRightPlayListWidget;
    CBottomCtrlBar* m_pBottomCtrlBar;

    std::unique_ptr<CVlcKits> m_pVlc;
    bool m_isPlay = false;
};
