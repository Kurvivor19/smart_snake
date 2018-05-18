#include <stdio.h>
#include <tchar.h>



// TODO: Установите здесь ссылки на дополнительные заголовки, требующиеся для программы

#pragma once
#ifndef _NODE_H_
#define _NODE_H_

struct coord
{
	int x;
	int y;
};

struct node
{
	struct coord data;
	struct node *tail;
};

struct seed
{
	struct coord data;
	//struct node *tail;
};

struct node* make_node(int x, int y);

#endif
