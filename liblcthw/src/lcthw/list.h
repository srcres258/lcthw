#ifndef LCTHW_LIST_H
#define LCTHW_LIST_H

#include <stdlib.h>

struct ListNode;

typedef struct ListNode {
    /**
     * ListNode *next指针，它指向另一个储存下一个元素的ListNode。
     */
    struct ListNode *next;
    /**
     * ListNode *prev指针，它指向另一个储存上一个元素的ListNode。
     */
    struct ListNode *prev;
    /**
     * 值，它是无类型的指针，存储我们想在链表中放置的东西。
     */
    void *value;
} ListNode;

typedef struct List {
    int count;
    ListNode *first;
    ListNode *last;
} List;

/**
 * 简单地创建主要的List结构。
 */
List *List_create(void);
/**
 * 销毁List以及其中含有的所有元素。
 */
void List_destroy(List *list);
/**
 * 为释放每个节点中的值（而不是节点本身）创建的辅助函数。
 */
void List_clear(List *list);
/**
 * 清理并销毁链表。它并不十分搞笑因为它对每个元素遍历两次。
 */
void List_clear_destroy(List *list);

/**
 * 返回链表中元素数量，它在元素添加或移除时维护。
 */
#define List_count(A) ((A)->count)
/**
 * 返回链表的首个元素，但是并不移除它。
 */
#define List_first(A) ((A)->first != NULL ? (A)->first->value : NULL)
/**
 * 返回链表的最后一个元素，但是不移除它。
 */
#define List_last(A) ((A)->last != NULL ? (A)->last->value : NULL)

/**
 * 第一个操作演示了链表的有点。它向链表尾添加新的元素，由于只是一些指针赋值，所以非常快。
 */
void List_push(List *list, void *value);
/**
 * List_push的反向版本，它去除最后一个元素并返回它。
 */
void *List_pop(List *list);

/**
 * 亦可以轻易对链表执行的另一件事，就是快速地向链表头部添加元素。由于找不到合适的词，这里我把它称为unshift。
 */
void List_unshift(List *list, void *value);
/**
 * 类似List_pop，但是它移除链表的首个元素并返回。
 */
void *List_shift(List *list);

/**
 * 当你执行List_pop或List_shift时，它执行实际的移除操作。在数据结构中移除数据总是看似比较困难，这个函数也
 * 不例外。它需要处理一些条件，取决于被移除的位置，在开头、在结尾、开头并且结尾，或者在中间。
 */
void *List_remove(List *list, ListNode *node);

/**
 * 遍历链表中的元素。
 */
#define LIST_FOREACH(L, S, M, V) ListNode *_node = NULL; \
    ListNode *V = NULL; \
    for (V = _node = L->S; _node != NULL; V = _node = _node->M)

#endif /* LCTHW_LIST_H */