
// MFCApplication3View.h : CMFCApplication3View Ŭ������ �������̽�
//

#pragma once

#include "resource.h"
#include "afxwin.h"
#include "GameAlgorithm.h"

class CMFCApplication3View : public CFormView
{
protected: // serialization������ ��������ϴ�.
	CMFCApplication3View();
	DECLARE_DYNCREATE(CMFCApplication3View)

public:
	enum{ IDD = IDD_MFCAPPLICATION3_FORM };

// Ư���Դϴ�.
public:
	CMFCApplication3Doc* GetDocument() const;

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	virtual void OnInitialUpdate(); // ���� �� ó�� ȣ��Ǿ����ϴ�.

// �����Դϴ�.
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

// ������ �޽��� �� �Լ�
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

#ifndef _DEBUG  // MFCApplication3View.cpp�� ����� ����
inline CMFCApplication3Doc* CMFCApplication3View::GetDocument() const
   { return reinterpret_cast<CMFCApplication3Doc*>(m_pDocument); }
#endif

