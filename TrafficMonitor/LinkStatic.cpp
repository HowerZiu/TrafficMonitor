// CLinkStatic.cpp: 实现文件

#include "stdafx.h"
#include "TrafficMonitor.h"
#include "LinkStatic.h"


// CLinkStatic

IMPLEMENT_DYNAMIC(CLinkStatic, CStatic)

CLinkStatic::CLinkStatic()
{

}

CLinkStatic::~CLinkStatic()
{
}


void CLinkStatic::SetBackgroundColor(COLORREF background_color)
{
    m_back_color = background_color;
}

void CLinkStatic::SetText(CString strText)
{
	m_text = strText;
	SetWindowText(m_text);
	RedrawWindow();
}

void CLinkStatic::SetURL(CString strURL)
{
	m_strURL = strURL;
}

CString CLinkStatic::GetURL() const
{
	return m_strURL;
}


BEGIN_MESSAGE_MAP(CLinkStatic, CStatic)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEHOVER()
	ON_WM_MOUSELEAVE()
	ON_WM_SETCURSOR()
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CLinkStatic 消息处理程序

void CLinkStatic::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	TRACKMOUSEEVENT tme;
	tme.cbSize = sizeof(tme);
	tme.hwndTrack = m_hWnd;
	tme.dwFlags = TME_LEAVE | TME_HOVER;
	tme.dwHoverTime = 1;
	_TrackMouseEvent(&tme);
}


void CLinkStatic::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
		if (m_link_is_url)
		{
			if (!m_strURL.IsEmpty())
				ShellExecute(NULL, _T("open"), m_strURL, NULL, NULL, SW_SHOW);	//打开超链接
		}
		else
		{
			CWnd* pParent{ GetParent() };
			if (pParent != nullptr)
				pParent->SendMessage(WM_LINK_CLICKED, (WPARAM)this);
		}
}


void CLinkStatic::OnMouseHover(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (!m_bHot)
	{
		m_bHot = true;
		Invalidate();
	}
	else
	{
		CStatic::OnMouseHover(nFlags, point);
	}
}


void CLinkStatic::OnMouseLeave()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_bHot = false;
	Invalidate();
}


BOOL CLinkStatic::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_bHot)
	{
		::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(32649)));
		return TRUE;
	}

	return CStatic::OnSetCursor(pWnd, nHitTest, message);
}



void CLinkStatic::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 在此处添加消息处理程序代码
					   // 不为绘图消息调用 CStatic::OnPaint()
	//准备工作
	CFont* pFont = GetFont();
	CFont font;
	if (pFont != NULL)
	{
		LOGFONT lf;
		pFont->GetLogFont(&lf);
		lf.lfUnderline = m_bHot;
		if (font.CreateFontIndirect(&lf))
			dc.SelectObject(font);
	}
	dc.SetTextColor(GetSysColor(COLOR_HOTLIGHT));
	dc.SetBkMode(TRANSPARENT);
	CRect rect;
	this->GetClientRect(&rect);
	//画背景
	dc.FillSolidRect(rect, m_back_color);
	///输出文字
	if (m_text.GetLength()>0)
	{
		dc.DrawText(m_text, rect, DT_VCENTER | DT_SINGLELINE);
	}
	font.DeleteObject();
}


void CLinkStatic::PreSubclassWindow()
{
	// TODO: 在此添加专用代码和/或调用基类
	DWORD dwStyle = GetStyle();
	::SetWindowLong(GetSafeHwnd(), GWL_STYLE, dwStyle | SS_NOTIFY);
	GetWindowText(m_text);

	CStatic::PreSubclassWindow();
}
