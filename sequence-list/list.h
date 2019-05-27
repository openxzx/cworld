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
    char *elem;
    int lenght;
}list_node_type;

extern list_node_type list_create(char *s);
extern int list_insert(char * elem, int index, char data, int len);
extern int list_delete(char * elem, int index, int len);

#endif
