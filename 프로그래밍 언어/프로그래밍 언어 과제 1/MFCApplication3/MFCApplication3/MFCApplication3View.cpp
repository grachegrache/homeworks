
// MFCApplication3View.cpp : CMFCApplication3View Ŭ������ ����
//

#include "stdafx.h"
#include <afxdlgs.h>
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "MFCApplication3.h"
#endif

#include "MFCApplication3Doc.h"
#include "MFCApplication3View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCApplication3View

IMPLEMENT_DYNCREATE(CMFCApplication3View, CFormView)

BEGIN_MESSAGE_MAP(CMFCApplication3View, CFormView)
	ON_BN_CLICKED(IDC_BUTTON1, &CMFCApplication3View::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CMFCApplication3View::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CMFCApplication3View::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CMFCApplication3View::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CMFCApplication3View::OnBnClickedButton5)
	ON_COMMAND_RANGE(IDC_BUTTON6, IDC_BUTTON21, &CMFCApplication3View::OnBnClickedStone)
	//ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON22, &CMFCApplication3View::OnBnClickedButton22)
END_MESSAGE_MAP()

// CMFCApplication3View ����/�Ҹ�

CMFCApplication3View::CMFCApplication3View()
	: CFormView(CMFCApplication3View::IDD)
{
	
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

}

CMFCApplication3View::~CMFCApplication3View()
{
	game_util.~GameAlgo();
}

void CMFCApplication3View::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RADIO1, radio_level3);

	DDX_Control(pDX, IDC_BUTTON6, stone[0][0]);
	DDX_Control(pDX, IDC_BUTTON7, stone[0][1]);
	DDX_Control(pDX, IDC_BUTTON8, stone[0][2]);
	DDX_Control(pDX, IDC_BUTTON9, stone[0][3]);
	DDX_Control(pDX, IDC_BUTTON10, stone[1][0]);
	DDX_Control(pDX, IDC_BUTTON11, stone[1][1]);
	DDX_Control(pDX, IDC_BUTTON12, stone[1][2]);
	DDX_Control(pDX, IDC_BUTTON13, stone[1][3]);
	DDX_Control(pDX, IDC_BUTTON14, stone[2][0]);
	DDX_Control(pDX, IDC_BUTTON15, stone[2][1]);
	DDX_Control(pDX, IDC_BUTTON16, stone[2][2]);
	DDX_Control(pDX, IDC_BUTTON17, stone[2][3]);
	DDX_Control(pDX, IDC_BUTTON18, stone[3][0]);
	DDX_Control(pDX, IDC_BUTTON19, stone[3][1]);
	DDX_Control(pDX, IDC_BUTTON20, stone[3][2]);
	DDX_Control(pDX, IDC_BUTTON21, stone[3][3]);
	DDX_Control(pDX, IDC_STATIC_TEXT, text_state);
}

BOOL CMFCApplication3View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CFormView::PreCreateWindow(cs);
}

void CMFCApplication3View::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

	//�ʱ�ȭ
	start = false;
	radio_level3.SetCheck(BST_CHECKED);
}


// CMFCApplication3View ����

#ifdef _DEBUG
void CMFCApplication3View::AssertValid() const
{
	CFormView::AssertValid();
}

void CMFCApplication3View::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CMFCApplication3Doc* CMFCApplication3View::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCApplication3Doc)));
	return (CMFCApplication3Doc*)m_pDocument;
}
#endif //_DEBUG


// CMFCApplication3View �޽��� ó����

//���� �����ϱ�
void CMFCApplication3View::OnBnClickedButton1()
{
	CFileDialog dlg(false, (LPCTSTR) _T("*.txt"), (LPCTSTR)NULL, (DWORD)OFN_EXPLORER | OFN_CREATEPROMPT, (LPCTSTR) _T("TXT Files(*.txt)|*.txt|"), (CWnd*)NULL, (DWORD)0, false);

	if (dlg.DoModal() == IDOK)
	{
		CString strPath = dlg.GetPathName();
		HANDLE hFile;
		DWORD dwWritten;
		char ch[120] = "";
		
		game_util.toString(ch);
		hFile = CreateFile(strPath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		WriteFile(hFile, ch, strlen(ch), &dwWritten, NULL);
		CloseHandle(hFile);

	}
}

//���� �ҷ�����
void CMFCApplication3View::OnBnClickedButton2()
{
	CFileDialog dlg(true, (LPCTSTR) _T("*.txt"), (LPCTSTR)NULL, (DWORD)OFN_EXPLORER | OFN_FILEMUSTEXIST, (LPCTSTR)_T("TXT Files(*.txt)|*.txt|"), (CWnd*)NULL, (DWORD)0, false);

	if (dlg.DoModal() == IDOK)
	{
		CString strPath = dlg.GetPathName();
		CStdioFile file;
		CFileException e;
		CString str, tmp;

		if (!file.Open(strPath, CFile::modeRead, &e)){
			e.ReportError();
			return;
		}

		while (file.ReadString(tmp))
			str.Append(tmp);

		game_util.reset();
		game_util.load(str);
		file.Close();

		//���� ���� ����
		start = false;
		game_util.setLevel(radio_level3.GetCheck() ? 3 : 5);
		game_util.setLastXY(x, y);

		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++){
				tmp.Format(_T("%c"), game_util.getState(i, j));
				stone[i][j].SetWindowTextW(tmp);
				if (game_util.getState(i, j) == 0)
					start |= game_util.getEnable(i, j);
			}

		if (start)
			changeTextState();
		else
			gameOver();
		
	}

}

//���� ����
void CMFCApplication3View::OnBnClickedButton3()
{
	start = true;
	game_util.reset();
	game_util.setLevel(radio_level3.GetCheck() ? 3 : 5);
	//clrBackStatic = RGB(128, 128, 128);
	//Invalidate();
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			stone[i][j].SetWindowTextW(_T(""));
	stone[1][1].SetWindowTextW(_T("W"));
	stone[1][2].SetWindowTextW(_T("B"));
	stone[2][1].SetWindowTextW(_T("B"));
	stone[2][2].SetWindowTextW(_T("W"));

	changeTextState();
}

//�Ѽ�������
void CMFCApplication3View::OnBnClickedButton4()
{
	if (text_state.GetWindowTextLengthW()){
		start = true;
		game_util.returnToPrevious();
		CString str;
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++){
				str.Format(_T("%c"), game_util.getState(i, j));
				stone[i][j].SetWindowTextW(str);
			}
		changeTextState();
	}
}

//���� ������
void CMFCApplication3View::OnBnClickedButton5()
{
	ExitMFCApp();
}

//�� Ŭ��
void CMFCApplication3View::OnBnClickedStone(UINT id){
	if (start){
		if (game_util.player_turn){

				int position[2] = { 0, 0 };
				bool found = setXYFromId(id);
				//clrBackStatic = RGB(256, 256, 256);

				if (found){
					if (!game_util.isEnd('B')){
						//���� �� ������
						if (game_util.getEnable(x, y)){
							//stone[x][y].Invalidate();
							placeStones();
							game_util.player_turn = false;

							//���� �� ��ǻ�Ͱ� ���� ���ϸ�
							if (game_util.isEnd('W')){
								//�÷��̾ ���� ���ϸ�
								if (game_util.isEnd('B')){
									gameOver();
									return;
								}
								game_util.player_turn = true;
							}
						}
					}

					changeTextState();
				}
		}
	}
}

//��ǻ�� ���� �� ����
void CMFCApplication3View::OnBnClickedButton22()
{
	if (start){
		if (game_util.player_turn == false){
			//�� �� ��ġ ��� ���� true ���� false
			if (game_util.calculatePosition(x, y)){
				//��ǻ�Ͱ� ���� �� ������
				//clrBackStatic = RGB(256, 256, 256);
				//stone[position[0]][position[1]].Invalidate();
				placeStones();
				game_util.player_turn = true;

				//���� �� �÷��̾ ���� ���ϸ�
				if (game_util.isEnd('B')){
					//��ǻ�͵� ���� ���ϸ�
					if (game_util.isEnd('W')){
						gameOver();
						return;
					}
					game_util.player_turn = false;

				}
			}
			changeTextState();
		}

	}
}
//
//HBRUSH CMFCApplication3View::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
//{
//	HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);
//	int id = pWnd->GetDlgCtrlID();
//	// TODO:  ���⼭ DC�� Ư���� �����մϴ�.
//	if (nCtlColor == CTLCOLOR_BTN)
//		if (id >= IDC_BUTTON6 && id <= IDC_BUTTON21)
//			pDC->SetBkColor(clrBackStatic);
//
//	// TODO:  �⺻���� �������� ������ �ٸ� �귯�ø� ��ȯ�մϴ�.
//	return hbr;
//}


void CMFCApplication3View::changeTextState(){
	//�����Ȳ �����ֱ�
	game_util.calculateScore();
	int b_score = game_util.getScore('B');
	int w_score = game_util.getScore('W');
	CString str;
	if (game_util.player_turn)
		str.Append(_T("�÷��̾�"));
	else
		str.Append(_T("��ǻ��"));
	str.AppendFormat(_T(" �����Դϴ�.\n��: %d ��: %d"), b_score, w_score);

	text_state.SetWindowTextW(str);
}

void CMFCApplication3View::gameOver(){
	//���� ��
	start = false;
	game_util.calculateScore();
	int b_score = game_util.getScore('B');
	int w_score = game_util.getScore('W');
	CString str;
	if (b_score > w_score)
		str = "�¸�";
	else if (b_score < w_score)
		str = "�й�";
	else
		str = "���º�";
	str.AppendFormat(_T(" �Դϴ�.\n��: %d ��: %d"), b_score, w_score);
	text_state.SetWindowTextW(str);
}

void CMFCApplication3View::placeStones(){
	//�� ����
	if (game_util.player_turn)
		game_util.placeStone(x, y);
	CString str;
	for (int k = 0; k < 4; k++)
		for (int l = 0; l < 4; l++){
			str.Format(_T("%c"), game_util.getState(k, l));
			stone[k][l].SetWindowTextW(str);
		}
}

bool CMFCApplication3View::setXYFromId(UINT id){
	bool found = false;
	//��ǥ ���ϱ�
	switch (id){
	case IDC_BUTTON6:
		found = true;
		x = 0;
		y = 0;
		break;
	case IDC_BUTTON7:
		found = true;
		x = 0;
		y = 1;
		break;
	case IDC_BUTTON8:
		found = true;
		x = 0;
		y = 2;
		break;
	case IDC_BUTTON9:
		found = true;
		x = 0;
		y = 3;
		break;
	case IDC_BUTTON10:
		found = true;
		x = 1;
		y = 0;
		break;
	case IDC_BUTTON11:
		found = true;
		x = 1;
		y = 1;
		break;
	case IDC_BUTTON12:
		found = true;
		x = 1;
		y = 2;
		break;
	case IDC_BUTTON13:
		found = true;
		x = 1;
		y = 3;
		break;
	case IDC_BUTTON14:
		found = true;
		x = 2;
		y = 0;
		break;
	case IDC_BUTTON15:
		found = true;
		x = 2;
		y = 1;
		break;
	case IDC_BUTTON16:
		found = true;
		x = 2;
		y = 2;
		break;
	case IDC_BUTTON17:
		found = true;
		x = 2;
		y = 3;
		break;
	case IDC_BUTTON18:
		found = true;
		x = 3;
		y = 0;
		break;
	case IDC_BUTTON19:
		found = true;
		x = 3;
		y = 1;
		break;
	case IDC_BUTTON20:
		found = true;
		x = 3;
		y = 2;
		break;
	case IDC_BUTTON21:
		found = true;
		x = 3;
		y = 3;
		break;
	default:
		break;
	}

	return found;
}

