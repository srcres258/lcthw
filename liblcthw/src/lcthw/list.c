#include <lcthw/list.h>
#include <lcthw/dbg.h>

List *List_create(void) {
    return calloc(1, sizeof(List));
}

void List_destroy(List *list) {
    LIST_FOREACH(list, first, next, cur) {
        if (cur->prev) {
            free(cur->prev);
        }
    }

    free(list->last);
    free(list);
}

void List_clear(List *list) {
    LIST_FOREACH(list, first, next, cur) {
        free(cur->value);
    }
}

void List_clear_destroy(List *list) {
    List_clear(list);
    List_destroy(list);
}

void List_push(List *list, void *value) {
    ListNode *node;

    node = calloc(1, sizeof(ListNode));
    check_mem(node);

    node->value = value;

    if (list->last) {
        list->last->next = node;
        node->prev = list->last;
        list->last = node;
    } else {
        list->first = node;
        list->last = node;
    }

    list->count++;

error:
    return;
}

void *List_pop(List *list) {
    ListNode *node;

    node = list->last;

    return node ? List_remove(list, node) : NULL;
}

void List_unshift(List *list, void *value) {
    ListNode *node;

    node = calloc(1, sizeof(ListNode));
    check_mem(node);

    node->value = value;

    if (list->first) {
        node->next = list->first;
        list->first->prev = node;
        list->first = node;
    } else {
        list->first = node;
        list->last = node;
    }

    list->count++;

error:
    return;
}

void *List_shift(List *list) {
    ListNode *node;

    node = list->first;

    return node ? List_remove(list, node) : NULL;
}

void *List_remove(List *list, ListNode *node) {
    ListNode *after, *before;
    void *result;

    result = NULL;

    check(list->first && list->last, "List is empty.");
    check(node, "Node can't be NULL.");

    if (node == list->first && node == list->last) {
        list->first = NULL;
        list->last = NULL;
    } else if (node == list->first) {
        list->first = node->next;
        check(list->first != NULL, "Invalid list, sonehow got a first that is NULL.");
        list->first->prev = NULL;
    } else if (node == list->last) {
        list->last = node->prev;
        check(list->last != NULL, "Invalid list, somehow got a next that is NULL.");
        list->last->next = NULL;
    } else {
        after = node->next;
        before = node->prev;
        after->prev = before;
        before->next = after;
    }

    list->count--;
    result = node->value;
    free(node);

error:
    return result;
}