#ifndef GAME_ALGORITHM_H
#define GAME_ALGORITHM_H


struct TreeNode{
	char turn;				//����
	int value;				//minimax Ʈ�������� "goodness" : �󸶳� ���� ���� ���� ������ �� �ִ°�
	int position_x;			//�� ��ġ
	int position_y;			//�� ��ġ
	TreeNode* parentNode;	//�θ� ���
	TreeNode* childNode;	//�ڽ� ���
	TreeNode* nextNode;		//���� �θ� ��带 ���� ���� ���

	TreeNode();				//������
	~TreeNode();			//�Ҹ���
	void addNode(const int goodness, const int x, const int y, const char turn);	//�ڽĳ�忡 ��带 �߰�
	void deleteChildNodes(TreeNode* current);	//��Ʈ ��带 ������ ��� �ڽ� ������ �����ϴ� �Լ�

};

class GameAlgo{
public:
	bool player_turn;	//true: �÷��̾� ����, false: ��ǻ�� ����

	GameAlgo();			//������
	~GameAlgo();		//�Ҹ���
	void calculateEnable(char state[4][4], char turn);	//�ǰ� ���ʸ� �Է����� �� �� �ִ� ���� enable�� ���
	bool calculatePosition(int i, int j);	//�÷��̾ ���� �ڸ��� �Է����� ��ǻ�Ͱ� ���� �ڸ��� ���
	void placeStone(int i, int j);			//�÷��̾ ���� �ڸ��� �Է¹޾� ���� ���� �ൿ(�� ������ ����)
	int getScore(char player);				//'B'�Ǵ� 'W'�� �Է����� ���� ����
	char getState(int i, int j);			//�ڸ��� �Է¹޾� ���� ���� �ڸ��� ��:'B' ��:'W' ����:0 ����
	bool getEnable(int i, int j);			//���� ������ ��� �������� enable�� ����ϰ� �Է� �ڸ��� enable ����
	void returnToPrevious();				//�Ѽ������� �ൿ
	void setLevel(int level);				//set level
	bool isEnd(char turn);					//'B'�Ǵ� 'W'�� �Է����� �� �� �ִ� �ڸ��� ������ true ���� �ƴϸ� false ����
	void reset();							//�ʱ�ȭ
	void calculateScore();					//��� ���� score�迭�� ���� ���
	void toString(char* str);				//char�� �����Ϳ� ���� ��Ȳ�� ����
	void load(CString str);					//CString �Է����� ���� ��Ȳ�� ����
	void setLastXY(int& x, int& y);			//�ҷ��������� �� history�� �������� ������ �÷��̾� �Է��� x,y������ ����

protected:
	int level;				//level
	int score[2];			//0��°: �÷��̾�, 1��°: ��ǻ��
	char state[4][4];		//���� ���� ��Ȳ, ù��°: �� �ι�°: ��
	bool enable[4][4];		//�ǿ� �� �� �ִ� ��: true ���� ��: false
	char history[100];		//������� ���� ���� ���
	struct TreeNode tree;	//minimax Ʈ���� ���� ����

	//state: �� ��Ȳ, i,j: ã�� �ڸ�, dx,dy: ã�� ����, find: Ž�� ó���� �� ���� ã���� true, �� ã���� false, turn: ����
	bool findEnable(char state[4][4], int i, int j, int dx, int dy, bool find, char turn) const;
	//�ڸ��� �� �� �ִ��� ã�� ����Լ�
	int findPossibleStones(char state[4][4], int i, int j, int dx, int dy, char turn) const;
	//���� �дٸ� �������� �� �ִ� �� ���� �����ϴ� ����Լ�
	void reverseStones(char state[4][4], int i, int j, int dx, int dy, char turn);
	//�� �ڸ��� �������� ���� ������ �Լ�
	void createTree(TreeNode* current, char state[4][4], char turn, int count);
	//Ʈ���� ���� ��Ȳ�� ������ ���� �� �� �ִ� �ڸ� ��带 �߰��ϴ� ����Լ�, count: Ʈ�� ����
	void makeMinimaxTree(TreeNode* current, int depth);
	//Ʈ���� depth ���̿� �ִ� ������ minimaxƮ���� ����� ����Լ�
	void addHistory(int x, int y);
	//history �迭�� �� �ڸ��� ����ϴ� �Լ�

};

void ExitMFCApp();
#endif