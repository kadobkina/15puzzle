#include <iostream>
#include <ctime>
#include <functional>
#include <iostream>
#include <unordered_set>
#include <queue>
#include <vector>
#include <random>

using namespace std;

const vector<size_t> ANSWER = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };

class Node
{
public:
	Node* parent; // �������� ������� ������
	vector<size_t> tag; // ������� ��������� ��������
	int emptyPos; // ������� ������ ������
	int curPos; // ������� ������� ������
	Node* startNode;
	Node* solve;

	Node(vector<size_t> vectTag, int empty = -1, int cur = -1, Node* parent = nullptr)
	{
		this->tag = move(vectTag);

		// ���� �� � ��������� ��������� ��������
		if (cur == -1)
		{
			empty = indOf(0) % 4;
			cur = indOf(0) / 4;
		}
		this->emptyPos = empty;
		this->curPos = cur;
		this->parent = parent;
	}

	// ��������� ���� ����������� ��������� �����
	vector<Node*> getPossibleSteps()
	{
		vector<Node*> possibleSteps;
		vector<size_t> nextTag;

		if (curPos > 0)
		{
			nextTag = tag;
			// ����������� ������� ������
			nextTag[emptyPos + curPos * 4] = nextTag[emptyPos + (curPos - 1) * 4];
			// ����������� ������ ������
			nextTag[emptyPos + (curPos - 1) * 4] = 0;
			// ��������� ����� ��������� ���
			possibleSteps.push_back(new Node(nextTag, emptyPos, curPos - 1, this));
		}
		if (curPos < 3)
		{
			nextTag = tag;
			nextTag[emptyPos + curPos * 4] = nextTag[emptyPos + (curPos + 1) * 4];
			nextTag[emptyPos + (curPos + 1) * 4] = 0;
			possibleSteps.push_back(new Node(nextTag, emptyPos, curPos + 1, this));
		}
		if (emptyPos > 0)
		{
			nextTag = tag;
			nextTag[emptyPos + curPos * 4] = nextTag[(emptyPos - 1) + curPos * 4];
			nextTag[(emptyPos - 1) + curPos * 4] = 0;
			possibleSteps.push_back(new Node(nextTag, emptyPos - 1, curPos, this));
		}
		if (emptyPos < 3)
		{
			nextTag = tag;
			nextTag[emptyPos + curPos * 4] = nextTag[(emptyPos + 1) + curPos * 4];
			nextTag[(emptyPos + 1) + curPos * 4] = 0;
			possibleSteps.push_back(new Node(nextTag, emptyPos + 1, curPos, this));
		}

		return possibleSteps;
	}

	// ������ �������� � �������
	int indOf(size_t el) {
		for (int i = 0; i < 16; i++)
			if (this->tag[i] == el)
				return i;
		return -1;
	}
};

// ������ ��������
void printTag(Node* t)
{
	vector<size_t>& vectTag = t->tag;
	int count = -1;
	for (int i = 0; i < 16; i++) {
		count++;
		if (count == 4) {
			cout << " |" << endl << " -----------------\n";
			count = 0;
		}
		if (vectTag[i] == 0)
			cout << " | " << " ";
		else if (vectTag[i] < 10)
			cout << " | " << vectTag[i];
		else
			switch (vectTag[i])
			{
			case 10:
				cout << " | " << "A";
				break;
			case 11:
				cout << " | " << "B";
				break;
			case 12:
				cout << " | " << "C";
				break;
			case 13:
				cout << " | " << "D";
				break;
			case 14:
				cout << " | " << "E";
				break;
			case 15:
				cout << " | " << "F";
				break;
			}
	}
	cout << " |\n\n -----------------\n\n";
}

// �������� �� ����������
bool isSolvable(Node* t)
{
	int count = 0;
	vector<size_t> vectTag = t->tag;

	for (int i = 0; i < 16; i++) {
		for (int j = i + 1; j < 16; j++)
			if ((vectTag[j] < vectTag[i]) && vectTag[i] && vectTag[j])
				count++;
	}

	return count % 2 != 0 && t->curPos % 2 == 0 || count % 2 == 0 && t->curPos % 2 != 0;
}

// �������� �������
bool gameOver(vector<size_t> vectTag)
{
	for (int i = 0; i < 15; i++)
		if (vectTag[i] != ANSWER[i])
			return false;
	return true;
}

// ������������� ���������� - ���������� �� ����� �� �� ����������� �����
int manhattanDistance(Node* t)
{
	vector<size_t>& vectTag = t->tag;
	int sum = 0;
	for (int i = 0; i < 16; i++) {
		if (vectTag[i] == 0)
			continue;
		int ind = (15 + vectTag[i]) % 16;
		sum += abs((i % 4) - ind % 4) + abs(i / 4 - ind / 4);
	}
	return sum;
}

// �������� A*
Node* AStar(Node* t)
{
	priority_queue<pair<int, Node*>> pq;
	unordered_set<Node*> passed;

	pq.push(pair<int, Node*>(manhattanDistance(t), t));

	while (!pq.empty())
	{
		pair<int, Node*> temp = pq.top();
		passed.insert(temp.second);
		pq.pop();

		if (gameOver(temp.second->tag)) {
			cout << "������!\n\n";
			return temp.second;
		}

		// ����� ���� ������
		int len = -manhattanDistance(temp.second) - temp.first;

		vector<Node*> steps = temp.second->getPossibleSteps();

		for (size_t i = 0; i < steps.size(); i++)
		{
			if (passed.find(steps[i]) == passed.end())
				pq.emplace(-(manhattanDistance(steps[i]) + len + 1), steps[i]);
		}

	}
	return 0;
}

// ����������� �������� IDA*
//pair<int, Node*> IDAStarIterative(Node* t, int deep)
//{
//	priority_queue<pair<int, Node*>> pq;
//	unordered_set<Node*> passed;
//
//	pq.push(pair<int, Node*>(manhattanDistance(t), t));
//
//	while (!pq.empty())
//	{
//		pair<int, Node*> temp = pq.top();
//		passed.insert(temp.second);
//		pq.pop();
//
//		if (gameOver(temp.second->tag)) {
//			cout << "������!\n\n";
//			return temp;
//		}
//
//		// ����� ���� ������
//		int len = -manhattanDistance(temp.second) - temp.first;
//
//		// �������� ���������� �������� ������� ������
//		if (len + 1 >= deep) return pair<int, Node*>(0, nullptr);
//
//		vector<Node*> steps = temp.second->getPossibleSteps();
//		for (size_t i = 0; i < steps.size(); i++)
//		{
//			if (passed.find(steps[i]) == passed.end())
//				pq.emplace(-(manhattanDistance(steps[i]) + len + 1), steps[i]);
//		}
//	}
//	return pair<int, Node*>(0, nullptr);
//}


/// <summary>
/// https://en.wikipedia.org/wiki/Iterative_deepening_A*
/// </summary>
/// <param name="node"></param>
/// <param name="g"></param>
/// <param name="deep"></param>
/// <returns></returns>
pair<int, bool> IDAStarIterative(Node* node, int g, int deep)
{
	int min = INT_MAX;
	if (node != nullptr)
	{
		int f = g + manhattanDistance(node);
		if (f > deep)
			return make_pair(f, false);

		vector<size_t>& vectTag = node->tag;
		if (gameOver(vectTag))
		{
			cout << "������!\n\n";
			return make_pair(f, true);
		}

		vector<Node*> steps = node->getPossibleSteps();
		for (size_t i = 0; i < steps.size(); i++)
		{
			pair<int, bool> res = IDAStarIterative(node, g + 1, deep);

			if (res.second)
			{
				cout << "������!\n\n";
				return res;
			}

			if (res.first < min)
				min = res.first;
		}
	}
	return make_pair(min, false);
}

pair<int, bool> IDAStar(Node* node)
{
	int deep = manhattanDistance(node);

	while (true)
	{
		pair<int, bool> res = IDAStarIterative(node->startNode, 0, deep);

		if (res.second) 
		{
			cout << "������!\n\n";
			return res;
		}
		deep = res.first;
	}
}

// �������� IDA*
//Node* IDAStar(Node* node)
//{
//	int deep = manhattanDistance(node);
//	while (true)
//	{
//		pair<int, Node*> res = IDAStarIterative(node, deep);
//		if (gameOver(res.second->tag) && res.second != nullptr)
//			return res.second;
//		deep = res.first;
//	}
//	return nullptr;
//}

int main()
{
	setlocale(LC_ALL, "rus");

	//string start = "123456789AFB EDC"; // ��� �������
	//string start = "F2345678A BE91CD"; // ��� �������
	//string start = "123456789ABCDEF "; // 0 �����
	//string start = "123456789ABCDE F"; // 1 ���
	string start = "1234 67859ACDEBF"; // 5 �����
	//string start = "51342 7896ACDEBF"; // 8 �����
	//string start = "16245A37 9C8DEBF"; // 10 �����
	//string start = "1723 68459ACDEBF"; // 13 �����
	//string start = "12345678A BE9FCD"; // 19 �����
	//string start = "512473 8A6BE9FCD"; // 27 �����,  AStar:  460 ����,    IDAStar: 493 ����,    IDAStarIterative: 512 ����
	//string start = "F2345678A BE91DC"; // 33 ����
	//string start = "751238 4A6BE9FCD"; // 35 �����,  AStar: 6368 ����,   IDAStar: 5994 ����,   IDAStarIterative: 4450 ����
	//string start = "75AB2C416D389F E"; // 45 �����
	//string start = " 4582E1DF79BCA36"; // 48 �����,  IDAStarIterative:  8941 ���� /  4064 ����
	//string start = "FE169B4C A73D852"; // 52 ����
	//string start = "D79F2E8A451 6C3B"; // 55 �����
	//string start = "DBE87A2C91F65 34"; // 58 �����   
	//string start = "BAC F478E19623D5"; // 61 ���

	vector<size_t> startTag;
	for (int i = 0; i < 16; i++)
	{
		if (start[i] == ' ')
			startTag.push_back((size_t)0);

		else if (isdigit(start[i]))
			startTag.push_back((size_t)(start[i] - '0'));

		else
			switch (start[i])
			{
			case 'A':
				startTag.push_back((size_t)10);
				break;
			case 'B':
				startTag.push_back((size_t)11);
				break;
			case 'C':
				startTag.push_back((size_t)12);
				break;
			case 'D':
				startTag.push_back((size_t)13);
				break;
			case 'E':
				startTag.push_back((size_t)14);
				break;
			case 'F':
				startTag.push_back((size_t)15);
				break;
			default:
				throw "������������ ������";
			}
	}

	Node* firstPosTag = new Node(startTag);
	firstPosTag->startNode = firstPosTag;

	// �������� �� ������� �������
	if (gameOver(startTag))
	{
		cout << "��� ������!\n\n";
		return 0;
	}
	// �������� �� ����������
	if (!isSolvable(firstPosTag))
		cout << "���������\n\n";
	// ���� �������
	else
	{
		printTag(firstPosTag);
		firstPosTag->parent = nullptr;

		// ������ ������� �������
		double startTime = clock();
		srand((unsigned)time(0));

		//Node* res = AStar(firstPosTag);
		pair<int, bool> res = IDAStar(firstPosTag);
		//pair<int, bool> res = IDAStarIterative(firstPosTag, 5);

		//printTag(res);

		// ���������� ����� �� �������
		int stepsCount = 0;
		//while (res != nullptr)
		//{
		//	res = res->parent;
		//	stepsCount++;
		//}

		// ����� ������� �������
		int endTime = (int)(((double)clock() - startTime) / CLOCKS_PER_SEC * 1e3);

		//printf("���������� ����� �� �������: %d, ����� ����������: %d ����\n", stepsCount - 1, endTime);
		printf("���������� ����� �� �������: %d, ����� ����������: %d ����\n", res.first, endTime);

	}
	return 0;
}
