#include <iostream>
#include <ctime>


const int MAX_LEVEL = 10;

struct Node
{
	int key;
	Node* forward[MAX_LEVEL];
};

struct SkipList
{
	int nowLevel;
	Node* head;
};


void InitSkipList(SkipList*& list)
{
	list = new SkipList;
	list->nowLevel = 0;
	list->head = new Node;
	for (int j = 0; j < MAX_LEVEL; j++)
	{
		list->head->forward[j] = nullptr;
	}
}


static int GetRandomLevel()
{
	int level = 1;
	for (;;)
	{
		if (rand() % 2 == 0)
			return level;
		level++;
		if (level == MAX_LEVEL - 2) return level;
	}
}


bool InsertSkipList(SkipList* list, int val)
{
	if (nullptr == list)
	{
		std::cout << "list is NULL" << std::endl;
		return false;
	}

	int level = list->nowLevel;
	Node* p = list->head;
	Node* q;
	Node* upDataNode[MAX_LEVEL];

	while (level >= 0)
	{
		q = p;
		while (nullptr != q && q->key < val)
		{
			p = q;
			q = p->forward[level];
		}

		if (NULL != q && q->key == val)
		{
			std::cout << "val has exist" << std::endl;
			return false;
		}
		upDataNode[level] = p;
		level--;
	}
	level = GetRandomLevel();
	if (level > list->nowLevel)
	{
		level = ++list->nowLevel;
		upDataNode[level] = list->head;
	}

	p = new Node;
	p->key = val;

	for (int j = 0; j < level; j++)
	{
		q = upDataNode[j];
		p->forward[j] = q->forward[j];
		q->forward[j] = p;
	}

	for (int j = level + 1; j < MAX_LEVEL; j++)
		p->forward[j] = nullptr;
	return true;
}


Node* FindSkipList(SkipList* list, int val, int& countRef)
{
	if (nullptr == list)
	{
		std::cout << "list is NULL" << std::endl;
		return NULL;
	}
	int level = list->nowLevel;
	Node* p = list->head;
	Node* q;

	while (level >= 0)
	{
		q = p->forward[level];
		++countRef;

		while (nullptr != q && q->key < val)
		{
			++countRef;
			p = q;
			q = p->forward[level];
		}
		if (NULL != q && q->key == val)
			return q;
		--level;
	}
	return NULL;
}


bool DeleteSkipList(SkipList* list, int val)
{
	if (nullptr == list)
	{
		std::cout << "list is NULL" << std::endl;
		return false;
	}
	int tempCount = 0;
	Node* ret = FindSkipList(list, val, tempCount);
	if (nullptr == ret) return false;

	int level = list->nowLevel;
	Node* p = list->head;
	Node* q;
	Node* upDateNodes[MAX_LEVEL];

	for (int j = 0; j < MAX_LEVEL; j++)
		upDateNodes[j] = nullptr;

	while (level >= 0)
	{
		q = p->forward[level];

		while (nullptr != q && q->key < val)
		{
			p = q;
			q = p->forward[level];
		}
		if (nullptr != q && q->key == val) upDateNodes[level] = p;
		--level;
	}

	for (int j = 0; j < list->nowLevel; j++)
	{
		q = upDateNodes[j];
		if (nullptr != q && q->forward[j] == ret)
		{
			q->forward[j] = ret->forward[j];
		}
	}
	delete ret;
	return true;
}

int main()
{
	srand((unsigned int)time(NULL));

	int val = 10;
	SkipList* list = NULL;
	InitSkipList(list);

	InsertSkipList(list, val);
	int count = 0;
	auto ret = FindSkipList(list, val, count);

	if (!ret) std::cout << "is NULL" << std::endl;
	else std::cout << ret->key << std::endl;

	DeleteSkipList(list, val);
	ret = FindSkipList(list, val, count);
	if (!ret) std::cout << "is NULL" << std::endl;
	return 0;
}