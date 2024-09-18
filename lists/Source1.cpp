#include <iostream>
#include <fstream>
#include "Header.h"
using namespace std;

char* decode(char* word);
char* substr(char* arr, int begin, int len);
int append(char* s, size_t size, char c);
char* get_menu(const char* menu_filename);
char* get_content();
char* parse(const char* buf);
void load_content(const char* template_name);
void show_list(sorted_list list, bool reversed);

void main()
{
	setlocale(LC_ALL, "Rus");
	cout << "Content-type: text/html; charset=windows-1251\n\n";
	load_content("lists.html");
}
char* decode(char* word)
{
	const int sz = 4096;
	char* new_str = new char[sz];
	new_str[0] = 0;
	char a;
	int j;

	for (int i = 0; i < strlen(word); i++)
	{
		if (word[i] == '+')
		{
			append(new_str, sz, ' ');
		}
		else if (word[i] == '%')
		{
			sscanf_s(substr(word, i + 1, 2), "%x", &j);
			a = static_cast<char>(j);
			append(new_str, sz, a);
			i = i + 2;
		}
		else
		{
			append(new_str, sz, word[i]);
		}
	}
	return (new_str);
}
char* substr(char* arr, int begin, int len)
{
	char* res = new char[len + 1];
	for (int i = 0; i < len; i++)
		res[i] = *(arr + begin + i);
	res[len] = 0;
	return res;
}
int append(char* s, size_t size, char c)
{
	if (strlen(s) + 1 >= size)
	{
		return 1;
	}
	int len = strlen(s);
	s[len] = c;
	s[len + 1] = '\0';
	return 0;
}
char* get_menu(const char* menu_filename)
{
	ifstream mf(menu_filename);
	const size_t res_sz = 65535;
	char* res = new char[res_sz];
	res[0] = 0;
	if (mf.is_open())
	{
		char* menuline = new char[1024];
		while (!mf.eof())
		{
			mf.getline(menuline, 1024);
			char* title;
			char* page = strtok_s(menuline, " ", &title);
			strcat_s(res, res_sz, "<div class='menuitem'>");
			strcat_s(res, res_sz, "<a href='");
			strcat_s(res, res_sz, page);
			strcat_s(res, res_sz, "'>");
			strcat_s(res, res_sz, title);
			strcat_s(res, res_sz, "</a></div>\n");
		}
		delete[] menuline;
		mf.close();
	}
	return res;
}
char* get_content()
{
	char* text = new char[40960];
	size_t len;
	errno_t err = _dupenv_s(&text, &len, "QUERY_STRING");
	sorted_list l1;

	if (strcmp(text, "") != 0)
	{
		char* next_token;
		char* exp = strtok_s(text, "=", &next_token);
		char* elems = strtok_s(NULL, "&", &next_token);
		char* v = strtok_s(NULL, "&", &next_token);
		char* act = strtok_s(NULL, "&", &next_token);
		char* past = strtok_s(NULL, "\n", &next_token);
		char* pch = strtok_s(elems, "+", &next_token);
		while ( pch != NULL)
		{
			int elem = atoi(pch);
			add(l1, elem);
			pch = strtok_s(NULL, "+", &next_token);
		}
		char* exp1 = strtok_s(v, "=", &next_token);
		char* a = strtok_s(NULL, "\n", &next_token);
		int value = atoi(a);
		char* action = strtok_s(act, "=", &next_token);
		if (strcmp(action, "add") == 0)
		{
			add(l1, value);
			show_list(l1, 0);
		}
		if (strcmp(action, "paste") == 0)
		{
			char* ress = strtok_s(past, "+", &next_token);
			char* i = strtok_s(NULL, "\n", &next_token);
			int index = atoi(i) - 1;
			insert(l1, index, value);
			show_list(l1, 0);
		}
	}
	const size_t res_sz = 65535;
	char* res = new char[res_sz];
	res[0] = 0;
	strcat_s(res, res_sz, "<form method='get' action='lists.cgi'>\n ");
	strcat_s(res, res_sz, "<label for='list'' class = 'form - ooo color - text''>Введите список: <br>\n");
	strcat_s(res, res_sz, "</label>\n");
	strcat_s(res, res_sz, "<textarea type='text' name='exp' id='exp' rows='2' cols='60' required></textarea><br>\n");
	strcat_s(res, res_sz, "<label for='exp'' class = 'form - ooo color - text''>Введите значение: <br>\n");
	strcat_s(res, res_sz, "</label>\n");
	strcat_s(res, res_sz, "<textarea type='text' name='exp' id='exp' rows='1' cols='20' required></textarea>\n");
	strcat_s(res, res_sz, "<div class='form - button''>\n");
	strcat_s(res, res_sz, "<div>\n <input type = \'checkbox\' id = \'add\' name = \'add\'>\n <label for = \'add\'> Добавить </label>\n </div>");
	strcat_s(res, res_sz, "<div>\n <input type = \'checkbox\' id = \'paste\' name = \'paste\'>\n <label for = \'paste\'> Вставить </label></div>");
	strcat_s(res, res_sz, "На какую позицию вставить\n");
	strcat_s(res, res_sz, "<textarea type='text' name='res' id='res' rows='1' cols='20'> </textarea>\n");
	strcat_s(res, res_sz, "<input type='submit' class='submit - button' value='Выполнить'>\n");
	strcat_s(res, res_sz, "</div>\n");
	strcat_s(res, res_sz, "</form>\n");

	return res;
}
char* parse(const char* buf)
{
	char* res;
	if (!strcmp(buf, "<!--menu-->"))
	{
		res = get_menu("menu.txt");
	}
	else if (!strcmp(buf, "<!--content-->"))
	{
		res = get_content();
	}
	else
	{
		res = new char[strlen(buf) + 1];
		strcpy_s(res, strlen(buf) + 1, buf);
	}
	return res;
}
void load_content(const char* template_name)
{
	ifstream tf(template_name);
	if (tf.is_open())
	{
		const size_t buf_sz = 65535;
		char* buf = new char[buf_sz];
		while (!tf.eof())
		{
			tf.getline(buf, buf_sz);
			char* parsed_buf = parse(buf);
			cout << parsed_buf << endl;
			delete[] parsed_buf;
		}
		delete[] buf;
		tf.close();
	}
}
void show_list(sorted_list list, bool reversed)
{
	auto* curr = (reversed) ? list.last : list.first;
	while (curr)
	{
		cout << curr->x << " ";
		curr = (reversed) ? curr->prev : curr->next;
	}
	cout << endl;
}