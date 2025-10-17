#include "rwlock.h"
#include <stdlib.h>
#include <pthread.h>

// Define the rwlock structure
struct rwlock {
    PRIORITY priority; // priority mode: READERS, WRITERS, or N_WAY
    uint32_t n_way_limit; // for N_WAY priority: maximum consecutive readers or writers

    // Synchronization primitives
    pthread_mutex_t mutex;
    pthread_cond_t readers_cond;
    pthread_cond_t writers_cond;

    // State variables
    uint32_t active_readers; // number of active readers
    uint32_t waiting_readers; // number of blocked readers
    uint32_t active_writers; // number of active writers
    uint32_t waiting_writers; // number of blocked writers
    uint32_t n_way_count; // counter for N_WAY priority
};

rwlock_t *rwlock_new(PRIORITY p, uint32_t n) {
    // allocate and initialize a new rwlock
    rwlock_t *rw = malloc(sizeof(*rw));
    if (rw == NULL) {
        return NULL; // memory allocation failed
    }

    // init default values
    rw->priority = p;
    rw->n_way_limit = n;
    rw->active_readers = 0;
    rw->waiting_readers = 0;
    rw->active_writers = 0;
    rw->waiting_writers = 0;
    rw->n_way_count = 0;

    if (pthread_mutex_init(&rw->mutex, NULL) != 0) {
        free(rw);
        return NULL; // mutex initialization failed
    }
    if (pthread_cond_init(&rw->readers_cond, NULL) != 0) {
        pthread_mutex_destroy(&rw->mutex);
        free(rw);
        return NULL; // condition variable initialization failed
    }
    if (pthread_cond_init(&rw->writers_cond, NULL) != 0) {
        pthread_cond_destroy(&rw->readers_cond);
        pthread_mutex_destroy(&rw->mutex);
        free(rw);
        return NULL; // condition variable initialization failed
    }
    return rw;
}

void rwlock_delete(rwlock_t **rwp) {
    if (rwp == NULL || *rwp == NULL)
        return; // check for NULL pointer
    rwlock_t *rw = *rwp;
    // destroy the mutex and condition variables
    pthread_mutex_destroy(&rw->mutex);
    pthread_cond_destroy(&rw->readers_cond);
    pthread_cond_destroy(&rw->writers_cond);
    // free the rwlock and null it's pointer
    free(rw);
    *rwp = NULL;
}

void reader_lock(rwlock_t *rw) {
    if (rw == NULL)
        return; // check for NULL pointer
    pthread_mutex_lock(&rw->mutex); // lock the mutex
    rw->waiting_readers++; // increment the number of waiting readers
    while (1) {
        // if a writer is active, wait
        if (rw->active_writers > 0) {
            pthread_cond_wait(&rw->readers_cond, &rw->mutex); // wait for the condition
        }
        // or if writers have priority and any are waiting, wait
        else if (rw->priority == WRITERS && rw->waiting_writers > 0) {
            pthread_cond_wait(&rw->readers_cond, &rw->mutex); // wait for the condition
        }
        // or if in N_WAY mode and writers are waiting and we've already granted n reads, wait{
        else if (rw->priority == N_WAY && rw->waiting_writers > 0
                 && rw->n_way_count >= rw->n_way_limit) {
            pthread_cond_wait(&rw->readers_cond, &rw->mutex); // wait for the condition
        } else {
            break; // no active writers
        }
    }

    rw->waiting_readers--; // decrement the number of waiting readers
    rw->active_readers++; // increment the number of active readers
    if (rw->priority == N_WAY)
        rw->n_way_count++; // increment the N_WAY counter
    pthread_mutex_unlock(&rw->mutex); // unlock the mutex
}

void reader_unlock(rwlock_t *rw) {
    if (rw == NULL)
        return; // check for NULL pointer
    pthread_mutex_lock(&rw->mutex); // lock the mutex
    rw->active_readers--; // decrement the number of active readers
    if (rw->active_readers == 0) { // if no active readers
        if (rw->priority == WRITERS) {
            if (rw->waiting_writers > 0) {
                pthread_cond_signal(&rw->writers_cond);
            } else if (rw->waiting_readers > 0) {
                pthread_cond_broadcast(&rw->readers_cond);
            }
        } else if (rw->priority == READERS) {
            if (rw->waiting_readers > 0) {
                pthread_cond_broadcast(&rw->readers_cond);
            } else if (rw->waiting_writers > 0) {
                pthread_cond_signal(&rw->writers_cond);
            }
        } else { // N_WAY
            if (rw->waiting_readers > 0 && rw->n_way_count < rw->n_way_limit) {
                // finish allowing n readers
                pthread_cond_broadcast(&rw->readers_cond);
            } else if (rw->waiting_writers > 0) {
                // Either we hit n, or no more readers—now writers go.
                pthread_cond_signal(&rw->writers_cond);
            } else if (rw->waiting_readers > 0) {
                // No writers, but some readers—let them in.
                pthread_cond_broadcast(&rw->readers_cond);
            }
        }
    }
    pthread_mutex_unlock(&rw->mutex); // unlock
}

void writer_lock(rwlock_t *rw) {
    if (rw == NULL)
        return; // check for NULL pointer
    pthread_mutex_lock(&rw->mutex);
    rw->waiting_writers++;
    while (1) {
        // wait if there are active readers or writers
        if (rw->active_writers > 0 || rw->active_readers > 0) {
            pthread_cond_wait(&rw->writers_cond, &rw->mutex);
        }
        // if readers are waiting and have priority, wait
        else if (rw->priority == READERS && rw->waiting_readers > 0) {
            pthread_cond_wait(&rw->writers_cond, &rw->mutex);
        }
        // otherwise if there are no active readers or writer and readers dont have priority than proceed
        else {
            break;
        }
    }
    rw->waiting_writers--;
    rw->active_writers = 1; // thread is locked for the 1 active writer
    pthread_mutex_unlock(&rw->mutex);
}

void writer_unlock(rwlock_t *rw) {
    if (rw == NULL)
        return; // check for NULL pointer
    pthread_mutex_lock(&rw->mutex); // lock the mutex
    rw->active_writers = 0; // reset the number of active writers

    if (rw->priority == READERS) {
        if (rw->waiting_readers > 0) {
            pthread_cond_broadcast(&rw->readers_cond);
        } else if (rw->waiting_writers > 0) {
            pthread_cond_signal(&rw->writers_cond);
        } // If no one waiting, broadcast to readers_cond
        else {
            pthread_cond_broadcast(&rw->readers_cond); // Wake any new readers
        }
    } else if (rw->priority == WRITERS) {
        if (rw->waiting_writers > 0) {
            pthread_cond_signal(&rw->writers_cond);
        } else if (rw->waiting_readers > 0) {
            pthread_cond_broadcast(&rw->readers_cond);
        } // (If no one waiting, signal writers_cond won't hurt, or do nothing)
        else {
            pthread_cond_signal(&rw->writers_cond); // Wake any new writers
        }
    } else { // N_WAY
        if (rw->n_way_limit > 0 && rw->waiting_readers > 0) {
            pthread_cond_broadcast(&rw->readers_cond);
        } else if (rw->waiting_writers > 0) {
            pthread_cond_signal(&rw->writers_cond);
        } else {
            pthread_cond_broadcast(&rw->readers_cond);
        }
        // Reset n_way_count for the *next* cycle of readers (or if no one was woken).
        rw->n_way_count = 0;
    }
    pthread_mutex_unlock(&rw->mutex);
}
