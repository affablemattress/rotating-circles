#pragma once
#include <stdlib.h>

#include "Circle.h"
#ifndef DATA_TYPE
	#define DATA_TYPE Circle
#endif

//------------------------Circular Doubly Linked List------------------------//

typedef struct Node {
	struct Node* pPrevious;
	struct DATA_TYPE* pData;
	struct Node* pNext;
}Node;

typedef struct List {
	Node* pHead;
	Node* pTail;
	int size;
}List;

List* ListCreate();
void ListAdd(List* pList, DATA_TYPE* pData);
void ListRemove(List* pList);

//-------------------------------List Iterator-------------------------------//

typedef struct Iterator {
	List* pIteratedList;
	Node* pCurrentNode;
}Iterator;

Iterator* IteratorCreate(List* pList);
DATA_TYPE* IteratorGet(Iterator* pIterator);
int IteratorNext(Iterator* pIterator);          //Returns 0 when list's ended. Otherwise returns 1.
int IteratorPrevious(Iterator* pIterator);

