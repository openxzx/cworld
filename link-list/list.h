/*
 * Author: Zhenxing Xu <xzxlnmail@163.com>
 *
 * This is free and unencumbered software released into the public domain.
 * For details see the UNLICENSE file at the root of the source tree.
 *
 */

#ifndef _LIST_H_
#define _LIST_H_

typedef struct list_node {
    unsigned int data;
    unsigned int index;
    struct list_node *pNext;
}list_node_type;

extern list_node_type *list_create(void);
extern void list_destroy(list_node_type *pHead);
extern int list_insert(list_node_type *pHead, unsigned int index, unsigned int data);
extern int list_delete(list_node_type *pHead, unsigned int index);
extern list_node_type *list_get(list_node_type *pHead, unsigned int index);
extern int list_traverse(list_node_type *pHead);
extern int check_index(list_node_type *pHead, unsigned int index);

#endif
