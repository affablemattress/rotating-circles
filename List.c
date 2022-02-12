
#include "List.h"

//------------------------Circular Doubly Linked List------------------------//

List* ListCreate() {
	List* pCreatedList = malloc(sizeof(List));
	pCreatedList->size = 0;
	return pCreatedList;
}

void ListAdd(List* pList, DATA_TYPE* pData) {
	Node* pCreatedNode = malloc(sizeof(Node));

	if (pList->size == 0) {
		pCreatedNode->pPrevious = pCreatedNode;
		pCreatedNode->pData = pData;
		pCreatedNode->pNext = pCreatedNode;

		pList->pHead = pCreatedNode;
		pList->pTail = pCreatedNode;
		pList->size = 1;
	}
	else {
		pCreatedNode->pPrevious = pList->pTail;
		pCreatedNode->pData = pData;
		pCreatedNode->pNext = pList->pHead;

		pList->pHead->pPrevious = pCreatedNode;
		pList->pTail->pNext = pCreatedNode;

		pList->pTail = pCreatedNode;
		pList->size += 1;
	}
}

void ListRemove(List* pList) {
	if (pList->size == 0) {
		return 0; //TO DO: Throw an error when trying to remove from an empty list.
	}
	else if (pList->size == 1) {
		free(pList->pTail);

		pList->pHead = 0;
		pList->pTail = 0;
		pList->size = 0;
	}
	else {
		Node* pNodeBuffer = pList->pTail;

		pList->pTail = pNodeBuffer->pPrevious;
		pNodeBuffer->pPrevious->pNext = pNodeBuffer->pNext;
		pNodeBuffer->pNext->pPrevious = pNodeBuffer->pPrevious;
		pList->size -= 1;

		free(pNodeBuffer);
	}

}

//-------------------------------List Iterator-------------------------------//

Iterator* IteratorCreate(List* pList) {                                   //Re-initialize the pIterator after modifiyng the pList.
	Iterator* pCreatedIterator = malloc(sizeof(Iterator));                //Or don't, I don't care future me >:(.

	pCreatedIterator->pIteratedList = pList;
	pCreatedIterator->pCurrentNode = pCreatedIterator->pIteratedList->pHead;
	return pCreatedIterator;
}

DATA_TYPE* IteratorGet(Iterator* pIterator) {
	return pIterator->pCurrentNode->pData;
}

int IteratorNext(Iterator* pIterator) {
	pIterator->pCurrentNode = pIterator->pCurrentNode->pNext;
	if (pIterator->pCurrentNode == pIterator->pIteratedList->pHead) {
		return 0;
	}
	else {
		return 1;
	}
}

int IteratorPrevious(Iterator* pIterator) {
	pIterator->pCurrentNode = pIterator->pCurrentNode->pPrevious;
	if (pIterator->pCurrentNode == pIterator->pIteratedList->pTail) {
		return 0;
	}
	else {
		return 1;
	}
}