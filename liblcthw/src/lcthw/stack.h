#ifndef LCTHW_STACK_H
#define LCTHW_STACK_H

#include <stdlib.h>

#define STACK_MAGIC 0x114514

typedef struct StackNode {
    void *value;
    struct StackNode *next;
} StackNode;

typedef struct Stack {
    StackNode *top;
    int count;
    int magic
} Stack;

/* 使用静态函数定义实现对栈的操作（因不能创建stack.c实现文件，只能通过宏定义来实现这些对栈的操作）。 */

static Stack *Stack_create(void) {
    Stack *stack;

    stack = malloc(sizeof(Stack));
    if (stack == NULL) {
        return NULL;
    }

    stack->top = NULL;
    stack->count = 0;

    return stack;
}

static void Stack_clear(Stack *stack) {
    StackNode *current, *next;

    current = stack->top;
    while (current) {
        next = current->next;
        free(current);
        current = next;
    }
}

static void Stack_destroy(Stack *stack) {
    if (stack) {
        Stack_clear(stack);
        free(stack);
    }
}

static int Stack_push(Stack *stack, void *value) {
    StackNode *node;

    node = malloc(sizeof(StackNode));
    if (node == NULL) {
        return 0;
    }

    node->value = value;
    node->next = stack->top;
    stack->top = node;
    stack->count++;

    return 1;
}

static void *Stack_pop(Stack *stack) {
    StackNode *node;
    void *value;
    
    if (!stack || !stack->top) {
        return NULL;
    }

    node = stack->top;
    value = node->value;
    stack->top = node->next;
    stack->count--;
    free(node);

    return value;
}

static inline void *Stack_peek(Stack *stack) {
    return (stack && stack->top) ? stack->top->value : NULL;
}

static inline int Stack_count(Stack *stack) {
    return stack ? stack->count : -1;
}

#define STACK_FOREACH(stack, cur) \
    for (StackNode *(cur) = (stack)->top; (cur) != NULL; (cur) = (cur)->next)

#endif /* LCTHW_STACK_H */