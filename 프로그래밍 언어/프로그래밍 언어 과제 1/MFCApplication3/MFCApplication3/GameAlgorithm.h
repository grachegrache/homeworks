#ifndef GAME_ALGORITHM_H
#define GAME_ALGORITHM_H


struct TreeNode{
	char turn;				//차례
	int value;				//minimax 트리에서의 "goodness" : 얼마나 많은 수의 돌을 뒤집을 수 있는가
	int position_x;			//행 위치
	int position_y;			//열 위치
	TreeNode* parentNode;	//부모 노드
	TreeNode* childNode;	//자식 노드
	TreeNode* nextNode;		//같은 부모 노드를 갖는 다음 노드

	TreeNode();				//생성자
	~TreeNode();			//소멸자
	void addNode(const int goodness, const int x, const int y, const char turn);	//자식노드에 노드를 추가
	void deleteChildNodes(TreeNode* current);	//루트 노드를 제외한 모든 자식 노드들을 제거하는 함수

};

class GameAlgo{
public:
	bool player_turn;	//true: 플레이어 차례, false: 컴퓨터 차례

	GameAlgo();			//생성자
	~GameAlgo();		//소멸자
	void calculateEnable(char state[4][4], char turn);	//판과 차례를 입력으로 둘 수 있는 곳을 enable에 계산
	bool calculatePosition(int i, int j);	//플레이어가 놓은 자리를 입력으로 컴퓨터가 놓을 자리를 계산
	void placeStone(int i, int j);			//플레이어가 놓은 자리를 입력받아 돌을 놓는 행동(돌 뒤집기 포함)
	int getScore(char player);				//'B'또는 'W'를 입력으로 점수 리턴
	char getState(int i, int j);			//자리를 입력받아 현재 판의 자리에 흑:'B' 백:'W' 없음:0 리턴
	bool getEnable(int i, int j);			//현재 차례인 사람 입장으로 enable을 계산하고 입력 자리의 enable 리턴
	void returnToPrevious();				//한수무르기 행동
	void setLevel(int level);				//set level
	bool isEnd(char turn);					//'B'또는 'W'를 입력으로 둘 수 있는 자리가 없으면 true 리턴 아니면 false 리턴
	void reset();							//초기화
	void calculateScore();					//멤버 변수 score배열에 점수 계산
	void toString(char* str);				//char형 포인터에 현재 상황을 저장
	void load(CString str);					//CString 입력으로 현재 상황을 적용
	void setLastXY(int& x, int& y);			//불러오기했을 때 history를 바탕으로 마지막 플레이어 입력을 x,y변수에 저장

protected:
	int level;				//level
	int score[2];			//0번째: 플레이어, 1번째: 컴퓨터
	char state[4][4];		//현재 판의 상황, 첫번째: 행 두번째: 열
	bool enable[4][4];		//판에 둘 수 있는 곳: true 없는 곳: false
	char history[100];		//현재까지 놓은 곳을 기록
	struct TreeNode tree;	//minimax 트리를 위한 변수

	//state: 판 상황, i,j: 찾을 자리, dx,dy: 찾을 방향, find: 탐색 처음에 적 돌을 찾으면 true, 못 찾으면 false, turn: 차례
	bool findEnable(char state[4][4], int i, int j, int dx, int dy, bool find, char turn) const;
	//자리에 둘 수 있는지 찾는 재귀함수
	int findPossibleStones(char state[4][4], int i, int j, int dx, int dy, char turn) const;
	//만약 둔다면 뒤집어질 수 있는 돌 수를 리턴하는 재귀함수
	void reverseStones(char state[4][4], int i, int j, int dx, int dy, char turn);
	//둔 자리를 기준으로 돌을 뒤집는 함수
	void createTree(TreeNode* current, char state[4][4], char turn, int count);
	//트리에 판의 상황과 순서에 따라 둘 수 있는 자리 노드를 추가하는 재귀함수, count: 트리 깊이
	void makeMinimaxTree(TreeNode* current, int depth);
	//트리의 depth 깊이에 있는 노드들을 minimax트리로 만드는 재귀함수
	void addHistory(int x, int y);
	//history 배열에 둔 자리를 기록하는 함수

};

void ExitMFCApp();
#endif