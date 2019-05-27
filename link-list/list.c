/*
 * Author: Zhenxing Xu <xzxlnmail@163.com>
 *
 * This is free and unencumbered software released into the public domain.
 * For details see the UNLICENSE file at the root of the source tree.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <assert.h>
#include "list.h"

/* Create List */
list_node_type *list_create(void)
{
	list_node_type *pHead;

	pHead = (list_node_type *)malloc(sizeof(list_node_type));
	if (pHead == NULL) {
		printf("\033[31mList init failed.\033[0m\n");
		return NULL;
	}

	pHead->data = 0xffffffff;
	pHead->index = 0xffffffff;
	pHead->pNext = NULL;

	printf("List create success.\n");

	return pHead;
}

/* Destroy List */
void list_destroy(list_node_type *pHead)
{
	list_node_type *pNode;
	list_node_type *pTemp;

	pNode = pHead->pNext;
	pHead->pNext = NULL;

	while (pNode != NULL) {
		pTemp = pNode->pNext;
		free(pNode);
		pNode = pTemp;
	}

	printf("List destroy success.\n");
}

/* Insert datas item into List Head */
int list_insert(list_node_type *pHead, unsigned int index, unsigned int data)
{
	list_node_type *pNode;

	if (check_index(pHead, index)) {
		printf("\033[31mList insert failed, it has have this index-%d.\033[0m\n", index);
		return -1;
	}

	pNode = (list_node_type *)malloc(sizeof(list_node_type));
	if (pNode == NULL) {
		printf("\033[31mList insert failed, malloc failed.\033[0m\n");
		return -2;
	}

	pNode->data = data;
	pNode->index = index;
	pNode->pNext = pHead->pNext;
	pHead->pNext = pNode;

	printf("List insert success.\n");

	return 0;
}

/* Delete datas item form list */
int list_delete(list_node_type *pHead, unsigned int index)
{
	list_node_type *pCur;
	list_node_type *pPrev;

	pPrev = pHead;
	pCur = pHead->pNext;

	while (pCur != NULL) {
		if (pCur->index == index) {
			pPrev->pNext = pCur->pNext;
			free(pCur);
			return 0;
		} else {
			pPrev = pCur;
			pCur = pCur->pNext;
		}
	}

	/* Do not have this index */
	printf("\033[31mDon't have this index.\033[0m\n");
	return -1;
}

/* Get datas item for list by index value */
list_node_type *list_get(list_node_type *pHead, unsigned int index)
{
	list_node_type *pTemp;

	pTemp = pHead->pNext;

	while (pTemp != NULL) {
		if (pTemp->index == index) {
			printf("list get success.\n");
			return pTemp;
		} else
			pTemp = pTemp->pNext;
	}

	printf("\033[31mList get failed no this index-%d.\033[0m\n", index);
	return NULL;
}

int list_traverse(list_node_type *pHead)
{
	list_node_type *pTemp;

	pTemp = pHead->pNext;

	if (pTemp == NULL) {
		printf("List is null.\n");
		return -1;
	}

	printf("\n");
	while (pTemp != NULL) {
		printf("\033[32mdata: %d, index: %d, pNext: %p\033[0m\n", pTemp->data, pTemp->index, pTemp->pNext);
		pTemp = pTemp->pNext;
	}
	printf("\n");

	return 0;
}

/* Checking list index whether it has */
int check_index(list_node_type *pHead, unsigned int index)
{
	list_node_type *pTemp;

	pTemp = pHead->pNext;

	while (pTemp != NULL) {
		if(pTemp->index == index) {
			printf("\033[32mList index has have.\033[0m\n");
			return -1;
		} else {
			pTemp = pTemp->pNext;
		}
	}

	return 0;
}
