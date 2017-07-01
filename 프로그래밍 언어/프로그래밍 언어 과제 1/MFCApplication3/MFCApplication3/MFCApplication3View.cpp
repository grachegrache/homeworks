
// MFCApplication3View.cpp : CMFCApplication3View 클래스의 구현
//

#include "stdafx.h"
#include <afxdlgs.h>
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
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

// CMFCApplication3View 생성/소멸

CMFCApplication3View::CMFCApplication3View()
	: CFormView(CMFCApplication3View::IDD)
{
	
	// TODO: 여기에 생성 코드를 추가합니다.

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
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CFormView::PreCreateWindow(cs);
}

void CMFCApplication3View::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

	//초기화
	start = false;
	radio_level3.SetCheck(BST_CHECKED);
}


// CMFCApplication3View 진단

#ifdef _DEBUG
void CMFCApplication3View::AssertValid() const
{
	CFormView::AssertValid();
}

void CMFCApplication3View::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CMFCApplication3Doc* CMFCApplication3View::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCApplication3Doc)));
	return (CMFCApplication3Doc*)m_pDocument;
}
#endif //_DEBUG


// CMFCApplication3View 메시지 처리기

//게임 저장하기
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

//게임 불러오기
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

		//게임 상태 적용
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

//게임 시작
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

//한수무르기
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

//게임 끝내기
void CMFCApplication3View::OnBnClickedButton5()
{
	ExitMFCApp();
}

//돌 클릭
void CMFCApplication3View::OnBnClickedStone(UINT id){
	if (start){
		if (game_util.player_turn){

				int position[2] = { 0, 0 };
				bool found = setXYFromId(id);
				//clrBackStatic = RGB(256, 256, 256);

				if (found){
					if (!game_util.isEnd('B')){
						//놓을 수 있으면
						if (game_util.getEnable(x, y)){
							//stone[x][y].Invalidate();
							placeStones();
							game_util.player_turn = false;

							//놓은 후 컴퓨터가 놓지 못하면
							if (game_util.isEnd('W')){
								//플레이어도 놓지 못하면
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

//컴퓨터 차례 돌 놓기
void CMFCApplication3View::OnBnClickedButton22()
{
	if (start){
		if (game_util.player_turn == false){
			//흰 돌 위치 계산 성공 true 실패 false
			if (game_util.calculatePosition(x, y)){
				//컴퓨터가 놓을 수 있으면
				//clrBackStatic = RGB(256, 256, 256);
				//stone[position[0]][position[1]].Invalidate();
				placeStones();
				game_util.player_turn = true;

				//놓은 후 플레이어가 놓지 못하면
				if (game_util.isEnd('B')){
					//컴퓨터도 놓지 못하면
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
//	// TODO:  여기서 DC의 특성을 변경합니다.
//	if (nCtlColor == CTLCOLOR_BTN)
//		if (id >= IDC_BUTTON6 && id <= IDC_BUTTON21)
//			pDC->SetBkColor(clrBackStatic);
//
//	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
//	return hbr;
//}


void CMFCApplication3View::changeTextState(){
	//현재상황 보여주기
	game_util.calculateScore();
	int b_score = game_util.getScore('B');
	int w_score = game_util.getScore('W');
	CString str;
	if (game_util.player_turn)
		str.Append(_T("플레이어"));
	else
		str.Append(_T("컴퓨터"));
	str.AppendFormat(_T(" 차례입니다.\n흑: %d 백: %d"), b_score, w_score);

	text_state.SetWindowTextW(str);
}

void CMFCApplication3View::gameOver(){
	//게임 끝
	start = false;
	game_util.calculateScore();
	int b_score = game_util.getScore('B');
	int w_score = game_util.getScore('W');
	CString str;
	if (b_score > w_score)
		str = "승리";
	else if (b_score < w_score)
		str = "패배";
	else
		str = "무승부";
	str.AppendFormat(_T(" 입니다.\n흑: %d 백: %d"), b_score, w_score);
	text_state.SetWindowTextW(str);
}

void CMFCApplication3View::placeStones(){
	//돌 놓기
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
	//좌표 구하기
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

