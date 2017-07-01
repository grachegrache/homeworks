
#include "stdafx.h"
#include "GameAlgorithm.h"
#include <string.h>

GameAlgo::GameAlgo(){
	reset();
}

GameAlgo::~GameAlgo(){
	TreeNode* top = &tree;
	tree.deleteChildNodes(top);
	tree.~TreeNode();
	level = 0;
}

//판과 차례를 입력으로 둘 수 있는 곳을 enable에 계산
void GameAlgo::calculateEnable(char state[4][4], char turn){
	bool find;

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++){
			find = false;
			//돌이 없으면
			if (state[i][j] == 0){
				//모든 방향으로 둘 수 있는지 탐색
				for (int k = -1; k < 2; k++)
					for (int l = -1; l < 2; l++)
						if (!(k == 0 && l == 0))
							find |= findEnable(state, i, j, k, l, false, turn);
			}
			enable[i][j] = find;
		}
}

//state: 판 상황, i,j: 찾을 자리, dx,dy: 찾을 방향, find: 탐색 처음에 적 돌을 찾으면 true, 못 찾으면 false, turn: 차례
//자리에 둘 수 있는지 찾는 재귀함수
bool GameAlgo::findEnable(char state[4][4], int i, int j, int dx, int dy, bool find, char turn) const{
	char enemy;
	enemy = turn == 'B' ? 'W' : 'B';

	if (i + dx >= 0 && i + dx < 4 && j + dy >= 0 && j + dy < 4){
		if (find){
			//첫 탐색에 적 돌을 찾으면
			if (state[i + dx][j + dy] == enemy)
				return findEnable(state, i + dx, j + dy, dx, dy, find, turn);
			else if (state[i + dx][j + dy] == turn)
				return true;
			else
				return false;
		}
		else{
			//첫 탐색
			if (state[i + dx][j + dy] == enemy)
				return findEnable(state, i + dx, j + dy, dx, dy, true, turn);
			else
				return false;
		}
	}
	return false;
}

//플레이어가 놓은 자리를 입력받아 돌을 놓는 행동(돌 뒤집기 포함)
void GameAlgo::placeStone(int i, int j){
	state[i][j] = 'B';
	addHistory(i, j);
	//뒤집기
	for (int k = -1; k < 2; k++)
		for (int l = -1; l < 2; l++){
			if (!(k == 0 && l == 0))
				if (findEnable(state, i, j, k, l, false, 'B'))
					reverseStones(state, i, j, k, l, 'B');
		}
}

//플레이어가 놓은 자리를 입력으로 컴퓨터가 놓을 자리를 계산
bool GameAlgo::calculatePosition(int i, int j){
	if (isEnd('W'))
		return false;

	int position[2] = { 0 };
	TreeNode* current = &tree;
	current->value = 0;
	current->position_x = i;
	current->position_y = j;

	//minimax 트리 생성
	createTree(current, state, 'W', 0);
	
	//루트 노드의 자리에 돌 놓기
	int value = current->value;
	current = current->childNode;
	while (current != 0){
		if (current->value == value){
			position[0] = current->position_x;
			position[1] = current->position_y;
			break;
		}
		current = current->nextNode;
	}
	state[position[0]][position[1]] = 'W';
	addHistory(position[0], position[1]);

	//뒤집기
	for (int m = -1; m < 2; m++)
		for (int n = -1; n < 2; n++){
			if (!(m == 0 && n == 0))
				if (findEnable(state, position[0], position[1], m, n, false, 'W'))
					reverseStones(state, position[0], position[1], m, n, 'W');
		}

	//트리 초기화
	tree.deleteChildNodes(current);
	tree.value = 0;
	tree.position_x = 0;
	tree.position_y = 0;
	tree.childNode = 0;
	tree.nextNode = 0;

	return true;
}

//만약 둔다면 뒤집어질 수 있는 돌 수를 리턴하는 재귀함수
int GameAlgo::findPossibleStones(char state[4][4], int i, int j, int dx, int dy, char turn) const{
	int count = 0;
	char enemy;
	enemy = turn == 'B' ? 'W' : 'B';

	while (i+dx >= 0 && i+dx < 4 && j+dy >= 0 && j+dy<4 && state[i + dx][j + dy] == enemy){
		count++;
		i += dx;
		j += dy;
	}

	return count;
}

//둔 자리를 기준으로 돌을 뒤집는 함수
void GameAlgo::reverseStones(char state[4][4], int i, int j, int dx, int dy, char turn){
	char enemy;
	enemy = turn == 'B' ? 'W' : 'B';

	while (i + dx >= 0 && i + dx < 4 && j + dy >= 0 && j + dy<4 && state[i + dx][j + dy] == enemy){
		state[i + dx][j + dy] = turn;
		i += dx;
		j += dy;
	}

}

//트리에 판의 상황과 순서에 따라 둘 수 있는 자리 노드를 추가하는 재귀함수, count: 트리 깊이
void GameAlgo::createTree(TreeNode* current, char state[4][4], char turn, int count){
	if (count >= level){
		//저장된 레벨의 깊이까지 도달하면
		//최하위 깊이부터 minimax 트리로 만듦
		TreeNode* top = &tree;
		for (int i = level - 1; i >= 0; i--)
			makeMinimaxTree(top, i);
		return;
	}

	if (count == 0 && current == 0)
		return;
	else if (count > 0 && current == 0){
		//지금까지 추가된 노드로 구성된 트리를 minimax 트리로 만듦
		TreeNode* top = &tree;
		for (int i = count - 1; i >= 0;i--)
			makeMinimaxTree(top, i);
		return;
	}

	int possible = 0;
	char enemy = turn == 'B' ? 'W' : 'B';
	//tmp 배열에 현재 노드의 자리에 돌을 놓음
	char tmp[4][4];
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			tmp[i][j] = state[i][j];
	tmp[current->position_x][current->position_y] = enemy;
	for (int k = -1; k < 2; k++)
		for (int l = -1; l < 2; l++){
			if (!(k == 0 && l == 0))
				if (findEnable(tmp,current->position_x,current->position_y,k,l,false,enemy))
					reverseStones(tmp, current->position_x, current->position_y, k, l, enemy);
		}
	//돌이 놓여진 tmp 배열에서 enable을 계산
	calculateEnable(tmp, turn);

	//판의 모든 자리에 대하여
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++){
			//현재 차례인 쪽이 놓을 수 있으면
			if (enable[i][j]){
				//모든 방향으로 뒤집을 수 있는 돌의 수를 더함
				possible = 0;
				for (int m = -1; m < 2; m++)
					for (int n = -1; n < 2; n++){
						if (!(m == 0 && n == 0))
							if (findEnable(tmp, i, j, m, n, false, turn))
								possible += findPossibleStones(tmp, i, j, m, n, turn);
					}
				if (possible != 0)
					current->addNode(possible, i, j, turn);
			}
		}

	//자식노드에 대해 재귀
	createTree(current->childNode, tmp, enemy, count + 1);
	//같은 깊이,부모의 노드에 대해 재귀
	createTree(current->nextNode, state, turn, count);
}

//트리의 depth 깊이에 있는 노드들을 minimax트리로 만드는 재귀함수
void GameAlgo::makeMinimaxTree(TreeNode* current, int depth){
	if (current == 0)
		return;
	if (depth <= 0 && current->childNode != 0){
		//주어진 깊이까지 도달하면
		TreeNode* next = current->childNode;
		TreeNode* value = new TreeNode;
		value->value = next->value;

		while (next != 0){
			if (next->turn == 'W'){
				//컴퓨터 차례이면 더 큰 value의 노드를 부모노드의 값으로 바꿈
				if (value->value < next->value)
					value->value = next->value;
			}
			else
				//플레이어 차례이면 더 작은 value의 노드를 부모노드의 값으로 바꿈
				if (value->value > next->value)
					value->value = next->value;
			next = next->nextNode;
		}
		current->value = value->value;
		delete value;
		return;
	}
	//같은 깊이 노드에 대해 재귀
	makeMinimaxTree(current->nextNode, depth);
	//자식 노드에 대해 재귀
	makeMinimaxTree(current->childNode, depth - 1);
}

//0번째 b 1번째 w
//멤버 변수 score배열에 점수 계산
void GameAlgo::calculateScore(){
	score[0] = 0;
	score[1] = 0;

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++){
			if (state[i][j] == 'B')
				score[0]++;
			else if (state[i][j] == 'W')
				score[1]++;
		}

}

//현재 차례인 사람 입장으로 enable을 계산하고 입력 자리의 enable 리턴
bool GameAlgo::getEnable(int i, int j){
	char turn = player_turn ? 'B' : 'W';
	calculateEnable(state, turn);
	return enable[i][j];
}

//자리를 입력받아 현재 판의 자리에 흑:'B' 백:'W' 없음:0 리턴
char GameAlgo::getState(const int i, const int j){
	return state[i][j];
}

//'B'또는 'W'를 입력으로 점수 리턴
int GameAlgo::getScore(const char player){
	return player == 'B' ? score[0] : score[1];
}

//불러오기했을 때 history를 바탕으로 마지막 플레이어 입력을 x,y변수에 저장
void GameAlgo::setLastXY(int& x, int& y){
	int index_x = -1;
	int index_y = -1;

	if (strlen(history)>5 && history[strlen(history) - 6] == 'B'){
		index_x = strlen(history) - 4;
		index_y = strlen(history) - 2;
	}
	else if (strlen(history) > 10){
		index_x = strlen(history) - 10;
		index_y = strlen(history) - 8;
	}
	if (index_x != -1 && index_y != -1){
		x = history[index_x] - '0';
		y = history[index_y] - '0';
	}
	
}

//한수무르기 행동
void GameAlgo::returnToPrevious(){
	//플레이어 차례:
	//컴퓨터가 놓은 자리에 플레이어가 놓았다고 가정하고 돌을 뒤집고 그 자리에 0을 대입
	//플레이어가 둔 차례까지 반복
	if (strlen(history)){
		int i, j, count;
		char turn, enemy;
		bool isChanged;
		count = 0;

		do{
			turn = history[strlen(history) - (6 * count) - 6];
			enemy = turn == 'B' ? 'W' : 'B';
			i = history[strlen(history) - (6 * count) - 4] - '0';
			j = history[strlen(history) - (6 * count) - 2] - '0';
			state[i][j] = 0;

			//마지막 돌을 제외한 뒤집기
			for (int k = -1; k < 2; k++)
				for (int l = -1; l < 2; l++){
					if (!(k == 0 && l == 0)){
						int tmp_i = i;
						int tmp_j = j;
						isChanged = false;

						while (tmp_i + k >= 0 && tmp_i + k < 4 && tmp_j + l >= 0 && tmp_j + l < 4 && state[tmp_i + k][tmp_j + l] == turn){
							state[tmp_i + k][tmp_j + l] = enemy;
							tmp_i += k;
							tmp_j += l;
							isChanged = true;
						}
						if (isChanged)
							state[tmp_i][tmp_j] = turn;
					}
				}

			if (turn == 'B'){
				history[strlen(history) - (6 * count) - 6] = 0;
				break;
			}
			count++;
		} while (strlen(history) >(6 * count));

		player_turn = true;
	}
}

void GameAlgo::setLevel(const int level){
	this->level = level;
}

//'B'또는 'W'를 입력으로 둘 수 있는 자리가 없으면 true 리턴 아니면 false 리턴
bool GameAlgo::isEnd(const char turn){
	calculateEnable(state, turn);
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			if (enable[i][j] == true)
				return false;
	return true;
}

//history 배열에 둔 자리를 기록하는 함수
void GameAlgo::addHistory(int x, int y){
	char turn = player_turn ? 'B' : 'W';
	char text_history[7];
	text_history[0] = turn;
	text_history[1] = '<';
	text_history[2] = '0' + x;
	text_history[3] = ',';
	text_history[4] = '0' + y;
	text_history[5] = '>';
	text_history[6] = 0;
	strcat_s(history, text_history);
}

//char형 포인터에 현재 상황을 저장
void GameAlgo::toString(char* str){
	int index = 0;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++){
			if (state[i][j] == 0)
				str[index++] = '0';
			else
				str[index++] = state[i][j];
		}
	str[index++] = ' ';
	str[index++] = player_turn ? 'B' : 'W';
	str[index++] = '\r';
	str[index++] = '\n';
	for (int i = 0; i < strlen(history); i++)
		str[index++] = history[i];
	str[index] = 0;
}

//CString 입력으로 현재 상황을 적용
void GameAlgo::load(CString str){
	int index = 0;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++){
			if (str.GetAt(index) == '0')
				state[i][j] = 0;
			else
				state[i][j] = str.GetAt(index);
			index++;
		}
	index++;
	player_turn = str.GetAt(index++) == 'B';
	
	for (int i = 0; str.GetAt(index) != 0; i++){
		history[i] = str.GetAt(index++);
		if (str.GetAt(index) == 0)
			history[++i] = 0;
	}

	/*for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			state[i][j] = state[i][j];*/

}

void GameAlgo::reset(){
	player_turn = true;
	level = 0;
	score[0] = 0;
	score[1] = 0;
	history[0] = 0;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++){
			state[i][j] = 0;
			enable[i][j] = false;
		}
	state[1][1] = 'W';
	state[1][2] = 'B';
	state[2][1] = 'B';
	state[2][2] = 'W';

	TreeNode* top = &tree;
	tree.deleteChildNodes(top);
	tree.value = 0;
	tree.turn = 0;
	tree.position_x = 0;
	tree.position_y = 0;
	tree.childNode = 0;
	tree.nextNode = 0;
}

TreeNode::TreeNode(){
	turn = 0;
	value = 0;
	position_x = 0;
	position_y = 0;
	childNode = 0;
	nextNode = 0;
	parentNode = 0;
}

TreeNode::~TreeNode(){
	turn = 0;
	value = 0;
	position_x = 0;
	position_y = 0;
	childNode = 0;
	nextNode = 0;
	parentNode = 0;
}

void TreeNode::addNode(const int goodness, const int x, const int y, const char turn){
	TreeNode* node = new TreeNode;
	node->turn = turn;
	node->value = goodness;
	node->position_x = x;
	node->position_y = y;
	node->childNode = 0;
	node->nextNode = 0;
	node->parentNode = this;

	if (childNode == 0)
		childNode = node;
	else{
		TreeNode* next = childNode;
		while (next->nextNode != 0)
			next = next->nextNode;
		next->nextNode = node;
	}

}

void TreeNode::deleteChildNodes(TreeNode* current){
	if (current == 0 || (current->childNode == 0 && current->parentNode == 0))
		return;
	if (current->nextNode == 0){
		if (current->childNode == 0)
			delete current;
		else
			deleteChildNodes(current->childNode);
	}else{
		if (childNode != 0)
			deleteChildNodes(current->childNode);
		deleteChildNodes(current->nextNode);
	}


}

void ExitMFCApp()
{
	// same as double-clicking on main window close box
	ASSERT(AfxGetMainWnd() != NULL);
	AfxGetMainWnd()->SendMessage(WM_CLOSE);
}
