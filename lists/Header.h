#pragma once
struct list_elem
{
	int x;
	list_elem* prev = nullptr;
	list_elem* next = nullptr;
};

struct sorted_list
{
	list_elem* first = nullptr;
	list_elem* last = nullptr;
	int length = 0;
};

void add(sorted_list& list, int value);
bool get(sorted_list list, int index, int& value);
bool contains(sorted_list list, int value);
bool remove_all(sorted_list& list, int value);
void clear(sorted_list& list);
void insert(sorted_list& list, int index, int value);