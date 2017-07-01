
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

//�ǰ� ���ʸ� �Է����� �� �� �ִ� ���� enable�� ���
void GameAlgo::calculateEnable(char state[4][4], char turn){
	bool find;

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++){
			find = false;
			//���� ������
			if (state[i][j] == 0){
				//��� �������� �� �� �ִ��� Ž��
				for (int k = -1; k < 2; k++)
					for (int l = -1; l < 2; l++)
						if (!(k == 0 && l == 0))
							find |= findEnable(state, i, j, k, l, false, turn);
			}
			enable[i][j] = find;
		}
}

//state: �� ��Ȳ, i,j: ã�� �ڸ�, dx,dy: ã�� ����, find: Ž�� ó���� �� ���� ã���� true, �� ã���� false, turn: ����
//�ڸ��� �� �� �ִ��� ã�� ����Լ�
bool GameAlgo::findEnable(char state[4][4], int i, int j, int dx, int dy, bool find, char turn) const{
	char enemy;
	enemy = turn == 'B' ? 'W' : 'B';

	if (i + dx >= 0 && i + dx < 4 && j + dy >= 0 && j + dy < 4){
		if (find){
			//ù Ž���� �� ���� ã����
			if (state[i + dx][j + dy] == enemy)
				return findEnable(state, i + dx, j + dy, dx, dy, find, turn);
			else if (state[i + dx][j + dy] == turn)
				return true;
			else
				return false;
		}
		else{
			//ù Ž��
			if (state[i + dx][j + dy] == enemy)
				return findEnable(state, i + dx, j + dy, dx, dy, true, turn);
			else
				return false;
		}
	}
	return false;
}

//�÷��̾ ���� �ڸ��� �Է¹޾� ���� ���� �ൿ(�� ������ ����)
void GameAlgo::placeStone(int i, int j){
	state[i][j] = 'B';
	addHistory(i, j);
	//������
	for (int k = -1; k < 2; k++)
		for (int l = -1; l < 2; l++){
			if (!(k == 0 && l == 0))
				if (findEnable(state, i, j, k, l, false, 'B'))
					reverseStones(state, i, j, k, l, 'B');
		}
}

//�÷��̾ ���� �ڸ��� �Է����� ��ǻ�Ͱ� ���� �ڸ��� ���
bool GameAlgo::calculatePosition(int i, int j){
	if (isEnd('W'))
		return false;

	int position[2] = { 0 };
	TreeNode* current = &tree;
	current->value = 0;
	current->position_x = i;
	current->position_y = j;

	//minimax Ʈ�� ����
	createTree(current, state, 'W', 0);
	
	//��Ʈ ����� �ڸ��� �� ����
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

	//������
	for (int m = -1; m < 2; m++)
		for (int n = -1; n < 2; n++){
			if (!(m == 0 && n == 0))
				if (findEnable(state, position[0], position[1], m, n, false, 'W'))
					reverseStones(state, position[0], position[1], m, n, 'W');
		}

	//Ʈ�� �ʱ�ȭ
	tree.deleteChildNodes(current);
	tree.value = 0;
	tree.position_x = 0;
	tree.position_y = 0;
	tree.childNode = 0;
	tree.nextNode = 0;

	return true;
}

//���� �дٸ� �������� �� �ִ� �� ���� �����ϴ� ����Լ�
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

//�� �ڸ��� �������� ���� ������ �Լ�
void GameAlgo::reverseStones(char state[4][4], int i, int j, int dx, int dy, char turn){
	char enemy;
	enemy = turn == 'B' ? 'W' : 'B';

	while (i + dx >= 0 && i + dx < 4 && j + dy >= 0 && j + dy<4 && state[i + dx][j + dy] == enemy){
		state[i + dx][j + dy] = turn;
		i += dx;
		j += dy;
	}

}

//Ʈ���� ���� ��Ȳ�� ������ ���� �� �� �ִ� �ڸ� ��带 �߰��ϴ� ����Լ�, count: Ʈ�� ����
void GameAlgo::createTree(TreeNode* current, char state[4][4], char turn, int count){
	if (count >= level){
		//����� ������ ���̱��� �����ϸ�
		//������ ���̺��� minimax Ʈ���� ����
		TreeNode* top = &tree;
		for (int i = level - 1; i >= 0; i--)
			makeMinimaxTree(top, i);
		return;
	}

	if (count == 0 && current == 0)
		return;
	else if (count > 0 && current == 0){
		//���ݱ��� �߰��� ���� ������ Ʈ���� minimax Ʈ���� ����
		TreeNode* top = &tree;
		for (int i = count - 1; i >= 0;i--)
			makeMinimaxTree(top, i);
		return;
	}

	int possible = 0;
	char enemy = turn == 'B' ? 'W' : 'B';
	//tmp �迭�� ���� ����� �ڸ��� ���� ����
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
	//���� ������ tmp �迭���� enable�� ���
	calculateEnable(tmp, turn);

	//���� ��� �ڸ��� ���Ͽ�
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++){
			//���� ������ ���� ���� �� ������
			if (enable[i][j]){
				//��� �������� ������ �� �ִ� ���� ���� ����
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

	//�ڽĳ�忡 ���� ���
	createTree(current->childNode, tmp, enemy, count + 1);
	//���� ����,�θ��� ��忡 ���� ���
	createTree(current->nextNode, state, turn, count);
}

//Ʈ���� depth ���̿� �ִ� ������ minimaxƮ���� ����� ����Լ�
void GameAlgo::makeMinimaxTree(TreeNode* current, int depth){
	if (current == 0)
		return;
	if (depth <= 0 && current->childNode != 0){
		//�־��� ���̱��� �����ϸ�
		TreeNode* next = current->childNode;
		TreeNode* value = new TreeNode;
		value->value = next->value;

		while (next != 0){
			if (next->turn == 'W'){
				//��ǻ�� �����̸� �� ū value�� ��带 �θ����� ������ �ٲ�
				if (value->value < next->value)
					value->value = next->value;
			}
			else
				//�÷��̾� �����̸� �� ���� value�� ��带 �θ����� ������ �ٲ�
				if (value->value > next->value)
					value->value = next->value;
			next = next->nextNode;
		}
		current->value = value->value;
		delete value;
		return;
	}
	//���� ���� ��忡 ���� ���
	makeMinimaxTree(current->nextNode, depth);
	//�ڽ� ��忡 ���� ���
	makeMinimaxTree(current->childNode, depth - 1);
}

//0��° b 1��° w
//��� ���� score�迭�� ���� ���
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

//���� ������ ��� �������� enable�� ����ϰ� �Է� �ڸ��� enable ����
bool GameAlgo::getEnable(int i, int j){
	char turn = player_turn ? 'B' : 'W';
	calculateEnable(state, turn);
	return enable[i][j];
}

//�ڸ��� �Է¹޾� ���� ���� �ڸ��� ��:'B' ��:'W' ����:0 ����
char GameAlgo::getState(const int i, const int j){
	return state[i][j];
}

//'B'�Ǵ� 'W'�� �Է����� ���� ����
int GameAlgo::getScore(const char player){
	return player == 'B' ? score[0] : score[1];
}

//�ҷ��������� �� history�� �������� ������ �÷��̾� �Է��� x,y������ ����
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

//�Ѽ������� �ൿ
void GameAlgo::returnToPrevious(){
	//�÷��̾� ����:
	//��ǻ�Ͱ� ���� �ڸ��� �÷��̾ ���Ҵٰ� �����ϰ� ���� ������ �� �ڸ��� 0�� ����
	//�÷��̾ �� ���ʱ��� �ݺ�
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

			//������ ���� ������ ������
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

//'B'�Ǵ� 'W'�� �Է����� �� �� �ִ� �ڸ��� ������ true ���� �ƴϸ� false ����
bool GameAlgo::isEnd(const char turn){
	calculateEnable(state, turn);
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			if (enable[i][j] == true)
				return false;
	return true;
}

//history �迭�� �� �ڸ��� ����ϴ� �Լ�
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

//char�� �����Ϳ� ���� ��Ȳ�� ����
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

//CString �Է����� ���� ��Ȳ�� ����
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
