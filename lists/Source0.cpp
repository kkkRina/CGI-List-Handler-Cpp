#include "Header.h"

void add(sorted_list& list, int value)
{
	if (contains(list, value))
	{
		return;
	}
	auto* new_el = new list_elem;
	new_el->x = value;
	list.length++;
	if (!list.first) list.first = list.last = new_el;
	else
	{
		if (value <= list.first->x)
		{
			new_el->next = list.first;
			list.first->prev = new_el;
			list.first = new_el;
		}
		else if (value >= list.last->x)
		{
			new_el->prev = list.last;
			list.last->next = new_el;
			list.last = new_el;
		}
		else
		{
			auto* curr = list.first;
			while (curr->next->x < value)
			{
				curr = curr->next;
			}
			curr->next->prev = new_el;
			new_el->next = curr->next;
			curr->next = new_el;
			new_el->prev = curr;
		}
	}
}
bool get(sorted_list list, int index, int& value)
{
	if (index < 0 || index >= list.length) return false;
	if (index == 0)
	{
		value = list.first->x;
		return true;
	}
	else if (index == list.length - 1)
	{
		value = list.last->x;
		return true;
	}
	else
	{
		auto center = (list.length - 1) / 2;
		bool forward = index <= center;
		auto* curr = (forward) ? list.first->next : list.last->prev;
		if (!forward) index = list.length - index - 1;
		int cnt = 0;
		while (++cnt < index)
			curr = (forward) ? curr->next : curr->prev;
		value = curr->x;
		return true;
	}
}
bool contains(sorted_list list, int value)
{
	if (!list.first || list.first->x > value || list.last->x < value) return false;
	auto* curr = list.first;
	while (curr->x < value)
	{
		curr = curr->next;
	}
	return curr->x == value;
}
bool remove_all(sorted_list& list, int value)
{
	if (!list.first || list.first->x > value || list.last->x < value)
		return false;
	auto* curr = list.first;
	while (curr && curr->x <= value)
	{
		if (curr->x == value)
		{
			if (curr->prev) curr->prev->next = curr->next;
			else list.first = list.first->next;
			if (curr->next) curr->next->prev = curr->prev;
			else list.last = list.last->prev;
			auto* old_el = curr;
			curr = curr->next;
			delete old_el;
		}
		else
			curr = curr->next;
	}
	return true;
}
void clear(sorted_list& list)
{
	auto* curr = list.first;
	while (curr)
	{
		auto* old_el = curr;
		curr = curr->next;
		delete old_el;
	}
	list.first = list.last = nullptr;
}
void insert(sorted_list& list, int index, int value) 
{
	if (contains(list, value)) 
	{
		return;
	}
	if (index < 0 || index > list.length) 
	{
		return;
	}
	auto* new_el = new list_elem;
	new_el->x = value;
	if (index == 0) 
	{
		new_el->next = list.first;
		list.first = new_el;
	}
	else 
	{
		list_elem* prev_el = list.first;
		for (int i = 0; i < index - 1; i++) 
		{
			prev_el = prev_el->next;
		}
		new_el->next = prev_el->next;
		prev_el->next = new_el;
	}
	list.length++;
}