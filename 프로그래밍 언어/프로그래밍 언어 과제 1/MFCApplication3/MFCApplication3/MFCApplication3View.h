
// MFCApplication3View.h : CMFCApplication3View 클래스의 인터페이스
//

#pragma once

#include "resource.h"
#include "afxwin.h"
#include "GameAlgorithm.h"

class CMFCApplication3View : public CFormView
{
protected: // serialization에서만 만들어집니다.
	CMFCApplication3View();
	DECLARE_DYNCREATE(CMFCApplication3View)

public:
	enum{ IDD = IDD_MFCAPPLICATION3_FORM };

// 특성입니다.
public:
	CMFCApplication3Doc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual void OnInitialUpdate(); // 생성 후 처음 호출되었습니다.

// 구현입니다.
public:
	bool start;
	int x, y;
	GameAlgo game_util;

	virtual ~CMFCApplication3View();
	bool setXYFromId(UINT id);
	void changeTextState();
	void placeStones();
	void gameOver();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	CButton radio_level3;
	CButton stone[4][4];
	COLORREF clrBackStatic;
	CStatic text_state;
	
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedStone(UINT id);
	afx_msg void OnBnClickedButton22();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//CStatic control_text;
	
};

#ifndef _DEBUG  // MFCApplication3View.cpp의 디버그 버전
inline CMFCApplication3Doc* CMFCApplication3View::GetDocument() const
   { return reinterpret_cast<CMFCApplication3Doc*>(m_pDocument); }
#endif

