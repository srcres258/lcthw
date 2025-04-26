#include <lcthw/list_algos.h>
#include <lcthw/list.h>
#include <lcthw/dbg.h>
#include <stdlib.h>

#ifdef LIST_ALGOS_USE_STANDARD_IMPLEMENTATION

/* 以下实现为文档中给出的标准答案。 */

static void ListNode_swap(ListNode *a, ListNode *b) {
    void *temp;

    temp = a->value;
    a->value = b->value;
    b->value = temp;
}

int List_bubble_sort(List *list, List_compare cmp) {
    int sorted;

    if (List_count(list) <= 1) {
        return 0; // already sorted
    }

    do {
        sorted = 1;
        LIST_FOREACH(list, first, next, cur) {
            if (cur->next) {
                if (cmp(cur->value, cur->next->value) > 0) {
                    ListNode_swap(cur, cur->next);
                    sorted = 0;
                }
            }
        }
    } while (!sorted);

    return 0;
}

static List *List_merge(List *left, List *right, List_compare cmp) {
    List *result;
    void *val;

    result = List_create();
    val = NULL;

    while (List_count(left) > 0 || List_count(right) > 0) {
        if (List_count(left) > 0 && List_count(right) > 0) {
            if (cmp(List_first(left), List_first(right)) <= 0) {
                val = List_shift(left);
            } else {
                val = List_shift(right);
            }

            List_push(result, val);
        } else if (List_count(left) > 0) {
            val = List_shift(left);
            List_push(result, val);
        } else if (List_count(right) > 0) {
            val = List_shift(right);
            List_push(result, val);
        }
    }

    return result;
}

List *List_merge_sort(List *list, List_compare cmp) {
    List *left, *right, *sort_left, *sort_right;
    int middle;

    if (List_count(list) <= 1) {
        return list;
    }

    left = List_create();
    right = List_create();
    middle = List_count(list) / 2;

    LIST_FOREACH(list, first, next, cur) {
        if (middle > 0) {
            List_push(left, cur->value);
        } else {
            List_push(right, cur->value);
        }

        middle--;
    }

    sort_left = List_merge_sort(left, cmp);
    sort_right = List_merge_sort(right, cmp);

    if (sort_left != left) {
        List_destroy(left);
    }
    if (sort_right != right) {
        List_destroy(right);
    }

    return List_merge(sort_left, sort_right, cmp);
}

#else

/*
以下实现为我自己编写的版本。

实现说明：

1. 冒泡排序：

    - 通过双重循环遍历链表，比较相邻节点的值。

    - 使用`swapped`标记优化，若某次遍历未发生交换则提前终止。

    - 直接交换节点的值指针，保持链表结构不变。

2. 归并排序：

    - 分割阶段：将链表元素均分到左右两个新链表（值拷贝方式保证原链表完整）。

    - 递归排序：对左右子链表递归调用归并排序。

    - 合并阶段：使用双指针法合并两个有序链表，生成新有序链表。

    - 内存管理：递归过程中创建的临时链表在合并后及时销毁。

测试适配性：

- 冒泡排序保持原链表结构，满足测试中多次排序的验证逻辑。

- 归并排序通过值拷贝方式保证原链表完整性，满足测试中销毁原链表的操作。

- 使用`List_push`创建新链表，确保双向链表的`prev/next`指针正确初始化。
*/

int List_bubble_sort(List *list, List_compare cmp) {
    /* 冒泡排序实现：通过交换相邻结点的值实现排序。 */

    int swapped;
    ListNode *cur, *temp;

    if (List_count(list) <= 1) {
        return 0;
    }

    do {
        swapped = 0;
        cur = list->first;

        while (cur->next) {
            if (cmp(cur->value, cur->next->value) > 0) {
                // 交换相邻结点的值。
                temp = cur->value;
                cur->value = cur->next->value;
                cur->next->value = temp;
                swapped = 1;
            }
            cur = cur->next;
        }
    } while (swapped);

    return 0;
}

static List *merge(List *left, List *right, List_compare cmp) {
    /* 归并排序辅助函数：合并两个有序链表。 */

    List *result;
    ListNode *l, *r;

    result = List_create();
    l = left->first;
    r = right->first;

    while (l && r) {
        if (cmp(l->value, r->value) <= 0) {
            List_push(result, l->value);
            l = l->next;
        } else {
            List_push(result, r->value);
            r = r->next;
        }
    }

    // 处理剩余结点。
    while (l) {
        List_push(result, l->value);
        l = l->next;
    }
    while (r) {
        List_push(result, r->value);
        r = r->next;
    }

    return result;
}

List *List_merge_sort(List *list, List_compare cmp) {
    /* 归并排序主逻辑。 */

    List *new_list, *left, *right, *left_sorted, *right_sorted, *merged;
    ListNode *cur;
    int mid, i;

    if (List_count(list) <= 1) {
        // 创建新链表拷贝单个元素（避免破坏原链表）。
        new_list = List_create();
        if (List_count(list) == 1) {
            List_push(new_list, list->first->value);
        }
        return new_list;
    }

    // 分割链表为左右两半。
    left = List_create();
    right = List_create();
    mid = List_count(list) / 2;
    cur = list->first;

    // 拷贝左半部分。
    for (i = 0; i < mid; i++) {
        List_push(left, cur->value);
        cur = cur->next;
    }
    // 拷贝右半部分。
    while (cur) {
        List_push(right, cur->value);
        cur = cur->next;
    }

    // 递归排序子链表。
    left_sorted = List_merge_sort(left, cmp);
    right_sorted = List_merge_sort(right, cmp);

    // 合并并清理临时链表。
    merged = merge(left_sorted, right_sorted, cmp);
    List_destroy(left);
    List_destroy(right);
    List_destroy(left_sorted);
    List_destroy(right_sorted);

    return merged;
}

#endif /* USE_STANDARD_IMPLEMENTATION */