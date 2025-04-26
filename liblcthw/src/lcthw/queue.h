#ifndef LCTHW_QUEUE_H
#define LCTHW_QUEUE_H

#include <stdlib.h>

typedef struct QueueNode {
    void *value;
    struct QueueNode *next;
} QueueNode;

typedef struct Queue {
    QueueNode *head;
    QueueNode *tail;
    int count;
} Queue;

static Queue *Queue_create(void) {
    Queue *queue;

    queue = malloc(sizeof(Queue));
    if (queue) {
        queue->head = NULL;
        queue->tail = NULL;
        queue->count = 0;
    }

    return queue;
}

static void Queue_destroy(Queue *queue) {
    QueueNode *current, *next;
    
    if (queue) {
        current = queue->head;
        while (current) {
            next = current->next;
            free(current);
            current = next;
        }
        free(queue);
    }
}

static void Queue_send(Queue *queue, void *value) {
    QueueNode *node;

    node = malloc(sizeof(QueueNode));
    if (node) {
        node->value = value;
        node->next = NULL;

        if (queue->tail) {
            queue->tail->next = node;
        } else {
            queue->head = node;
        }
        queue->tail = node;
        queue->count++;
    }
} 

static inline void *Queue_peek(Queue *queue) {
    return queue->head ? queue->head->value : NULL;
}

static void *Queue_recv(Queue *queue) {
    QueueNode *node;
    void *value;

    
    if (!queue->head) {
        return NULL;
    }

    node = queue->head;
    value = node->value;
    queue->head = node->next;

    if (!queue->head) {
        queue->tail = NULL;
    }

    queue->count--;
    free(node);

    return value;
}

static inline int Queue_count(Queue *queue) {
    return queue->count;
}

#define QUEUE_FOREACH(queue, cur) \
    for (QueueNode *(cur) = (queue)->head; (cur) != NULL; (cur) = (cur)->next)

#endif /* LCTHW_QUEUE_H */