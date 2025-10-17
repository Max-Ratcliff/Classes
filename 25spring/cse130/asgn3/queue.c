// Basic test to show that the queue API works in non-concurrent settings.
// By: Andrew Quinn

#include "queue.h"

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

struct queue {
    int max_size;
    int head;
    int tail;
    int size;

    void **elements;
    pthread_mutex_t lock;
    pthread_cond_t not_empty;
    pthread_cond_t not_full;
};

/** @brief Dynamically allocates and initializes a new queue with a
 *         maximum size, size
 *
 *  @param size the maximum size of the queue
 *
 *  @return a pointer to a new queue_t
 */
queue_t *queue_new(int size) {
    if (size <= 0) {
        return NULL;
    }

    queue_t *q = malloc(sizeof(queue_t));
    if (q == NULL) {
        return NULL;
    }

    q->max_size = size;
    q->head = 0;
    q->tail = 0;
    q->size = 0;

    q->elements = malloc(sizeof(void *) * size);
    if (q->elements == NULL) {
        free(q);
        return NULL;
    }

    pthread_mutex_init(&q->lock, NULL);
    pthread_cond_init(&q->not_empty, NULL);
    pthread_cond_init(&q->not_full, NULL);

    return q;
}

/** @brief Delete your queue and free all of its memory.
 *
 *  @param q the queue to be deleted.  Note, you should assign the
 *  passed in pointer to NULL when returning (i.e., you should set
 *  *q = NULL after deallocation).
 *
 */
void queue_delete(queue_t **q) {
    if (q == NULL || *q == NULL) {
        return;
    }

    queue_t *queue = *q;

    pthread_mutex_lock(&queue->lock);
    // Wake up any waiters so they don't deadlock
    pthread_cond_broadcast(&queue->not_empty);
    pthread_cond_broadcast(&queue->not_full);
    pthread_mutex_unlock(&queue->lock);

    pthread_cond_destroy(&queue->not_empty);
    pthread_cond_destroy(&queue->not_full);
    pthread_mutex_destroy(&queue->lock);

    free(queue->elements);
    free(queue);

    *q = NULL;
}

/** @brief push an element onto a queue
 *
 *  @param q the queue to push an element into.
 *
 *  @param elem th element to add to the queue
 *
 *  @return A bool indicating success or failure.  Note, the function
 *          should succeed unless the q parameter is NULL.
 */
bool queue_push(queue_t *q, void *elem) {
    if (q == NULL || elem == NULL) {
        return false;
    }

    pthread_mutex_lock(&q->lock);

    // Wait until the queue is not full
    while (q->size >= q->max_size) {
        pthread_cond_wait(&q->not_full, &q->lock);
    }

    q->elements[q->tail] = elem;
    q->tail = (q->tail + 1) % q->max_size;
    q->size++;

    // Signal that the queue is not empty
    pthread_cond_signal(&q->not_empty);
    pthread_mutex_unlock(&q->lock);
    return true;
}

/** @brief pop an element from a queue.
 *
 *  @param q the queue to pop an element from.
 *
 *  @param elem a place to assign the poped element.
 *
 *  @return A bool indicating success or failure.  Note, the function
 *          should succeed unless the q parameter is NULL.
 */
bool queue_pop(queue_t *q, void **elem) {
    if (q == NULL || elem == NULL) {
        return false;
    }

    pthread_mutex_lock(&q->lock);

    // Wait until the queue is not empty
    while (q->size <= 0) {
        pthread_cond_wait(&q->not_empty, &q->lock);
    }

    *elem = q->elements[q->head];
    q->head = (q->head + 1) % q->max_size;
    q->size--;

    // Signal that the queue is not full
    pthread_cond_signal(&q->not_full);
    pthread_mutex_unlock(&q->lock);
    return true;
}
