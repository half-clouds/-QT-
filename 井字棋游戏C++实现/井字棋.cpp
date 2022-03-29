#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
using namespace std;
typedef std::uint64_t hash_t;

constexpr hash_t prime = 0x100000001B3ull;
constexpr hash_t basis = 0xCBF29CE484222325ull;
constexpr unsigned long long operator "" _hash(char const* p, size_t);
constexpr hash_t hash_compile_time(char const* str, hash_t last_value = basis)
{
	return *str ? hash_compile_time(str + 1, (*str ^ last_value) * prime) : last_value;
}
constexpr unsigned long long operator "" _hash(char const* p, size_t)
{
	return hash_compile_time(p);
}

hash_t hash_(char const* str)
{
	hash_t ret{ basis };

	while (*str) {
		ret ^= *str;
		ret *= prime;
		str++;
	}

	return ret;
}
class Chess {
public:
	char chess[3][3] = { 0 };
	char player, computer;
	bool playerF;
	Chess() {
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				chess[i][j] = '-';
	}
	bool detectWin(char player) {//判断玩家player是否胜利
		for (int i = 0; i < 3; i++)
			if (chess[i][0] == player && chess[i][1] == player && chess[i][2] == player) {
				return true;
			}
			else {
				if (chess[0][i] == player && chess[1][i] == player && chess[2][i] == player)
					return true;
			}
		if (chess[0][0] == player && chess[1][1] == player && chess[2][2] == player)
			return true;
		if (chess[0][2] == player && chess[1][1] == player && chess[2][0] == player)
			return true;
		return false;
	}
	bool isEmpty() {//判断棋盘是否为空
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				if (chess[i][j] == '-')return false;  //还有可以下的地方就不空
			}
		}
		return true;
	}
	void startGame() {
		player = 'O';
		computer = 'X';
		cout << "请选择玩家符号X/O,(输入1代表选择X，输入2代表选择O,默认为O)：";
		int get; cin >> get;
		if (get == 1) {
			player = 'X';
			computer = 'O';
		}
		else if (get != 2) {
			cout << ("输入有误，默认为O");
		}
		playerF = true;
		cout << "是否先手1/2(1代表是，2代表否，默认是)：";
		cin >> get;
		if (get == 2)playerF = false;
		else if (get != 1) {
			cout << ("输入有误，默认玩家先手！");
		}
		//1是先手 F为true
		if (playerF) {
			Print(); //打印棋盘
		}
		//不先手
		else {
			//Random rand = new Random();
			srand((unsigned int)time(0));//初始化种子为随机值
			int startPoint = rand() % 9 ; // 0-8 
			int row;
			/*if (startPoint % 3 == 0)row = startPoint / 3;
			else row = startPoint / 3 + 1;
			int col = startPoint - (row - 1) * 3;*/
			int col;
			if (startPoint == 6 || startPoint == 7 || startPoint == 8)row = 2;
			if (startPoint == 3 || startPoint == 4 || startPoint == 5)row = 1;
			if (startPoint == 0 || startPoint == 1 || startPoint == 2)row = 0;
			if (startPoint == 0 || startPoint == 3 || startPoint == 6)col = 0;
			if (startPoint == 1 || startPoint == 4 || startPoint == 7)col = 1;
			if (startPoint == 2 || startPoint == 5 || startPoint == 8)col = 2;
			chess[row][col] = computer; //电脑下
			Print(); //打印棋盘
		}
	}
	void Print() {
		cout << ("-------------\n");
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				cout << ("| ");
				cout << chess[i][j] << " ";
				if (j == 2)cout << ("|") << endl;
			}
			if (i == 2)cout << ("-------------") << endl;
		}
	}
	void playerInput() {
		cout<<("轮到你走了，请输入棋的位置：\n");
		int row, col;
		
		while (true) {
			int num; cin>>num;
			if (num >= 0 && num <= 8) {
				if (num == 6 || num == 7 || num == 8)row = 2;
				if (num == 3 || num == 4 || num == 5)row = 1;
				if (num == 0 || num == 1 || num == 2)row = 0;
				if (num == 0 || num == 3 || num == 6)col = 0;
				if (num == 1 || num == 4 || num == 7)col = 1;
				if (num == 2 || num == 5 || num == 8)col = 2;
				if (chess[row][col] != '-')cout << ("该位置已有棋子");
				else break;
			}
			else {
				cout << ("输入有误，请重新输入");
				continue;
			}
		}
		//获取用户输入的row 和col
		chess[row][col] = player; //玩家下棋
		Print();
		//		input.close();
	}
	int bestInput(string state, string nextState, int alpha, int beta) {//输入，调用剪枝的过程
		char ch;
		if (state=="computer")ch = computer;
		else ch = player;
		if (detectWin(ch)) {
			if (state == "computer")return 1;
			else  return -1;
		}
		else if (isEmpty()) {
			return 0;
		}
		else {
			int score;
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					if (chess[i][j] == '-') {
						chess[i][j] = ch;
						Print();
						score = bestInput(nextState, state, alpha, beta);
						Print();
						chess[i][j] = '-';
						Print();
						if (state == "computer") {
							if (score >= alpha)alpha = score;
							if (alpha > beta)return beta;
						}
						else {
							if (score < beta)beta = score;
							if (beta <= alpha)return alpha;
						}
					}
				}
			}
			if (state == "computer")return alpha;
			else return beta;
		}
	}
	void computerInput() {
		int best = 0;
		int bestScore = -1000;
		int score;
		Print();
		for (int i = 0; i <=2; i++) {
			for (int j = 0; j <= 2; j++) {
				if (chess[i][j] == '-') {
					chess[i][j] = computer;
					Print();
					score = bestInput("player", "computer", -1000, 1000);//alpha-beta剪枝是一个根据上下界限剪枝的算法，初始的上下界限为无穷
					if (score > bestScore) {//在同一层的节点里面需要不断试探性递归，用回溯法找到最合适的下棋点使自己胜算最大
						bestScore = score;
						best = (i) * 3 + j;
					}
					Print();
					chess[i][j] = '-';
					Print();
				}
			}
		}
		int row, col;
		if (best == 6 || best == 7 || best == 8)row = 2;
		if (best == 3 || best == 4 || best == 5)row = 1;
		if (best == 0 || best == 1 || best == 2)row = 0;
		if (best == 0 || best == 3 || best == 6)col = 0;
		if (best == 1 || best == 4 || best == 7)col = 1;
		if (best == 2 || best == 5 || best == 8)col = 2;
		//获取位置
		Print();
		chess[row][col] = computer;
		Print();
	}
};

int main()
{
	Chess* c = new Chess;
	c->startGame();
	//char const* current = "player";
	string current = "player"; int flag;
	while (!c->detectWin(c->computer) && !c->detectWin(c->player) && !c->isEmpty()) {//终止条件是当前棋盘为空或者有一方胜利
		if (current == "player")flag = 1;
		else if (current == "computer")flag = 2;
		{
			switch (flag) {
			case 1:
				c->playerInput(); current = "computer"; break;//当玩家下完后轮到电脑下
			case 2:
				c->computerInput(); current = "player"; break;
			default:break;
			}
		}
	}
	if(c->detectWin(c->computer))cout<<("电脑胜利！\n");
		else if (c->detectWin(c->player))cout << ("玩家胜利\n");
		else cout << ("平局！\n");
	return 0;
}
