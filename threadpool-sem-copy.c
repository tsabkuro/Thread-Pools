#include <stdlib.h>

#include "uthread.h"
#include "uthread_mutex_cond.h"
#include "uthread_util.h"
#include "uthread_sem.h"

#include "threadpool.h"

#include <stdio.h>

typedef struct work_t {
  void (*thread_fun)(tpool_t, void *arg);
  void *arg;
  struct work_t *next;
} work_t;

struct tpool {
  /* TO BE COMPLETED BY THE STUDENT */
  uthread_t* threads;
  work_t* workFront;
  work_t* workBack;
  int threadCount;
  uthread_sem_t tasks;
  uthread_sem_t lock;
  int joined;
};


void enqueue (tpool_t pool, void* arg, void (*callback) (tpool_t, void*)) {

  // printf("tasks: %d\n", pool->tasks);
  work_t* newWork = malloc(sizeof(work_t));
  newWork->arg = arg;
  newWork->thread_fun = callback;
  newWork->next = NULL;

  if (!pool->workFront) {
    pool->workFront = newWork;
    pool->workBack = pool->workFront;
  } else {
    pool->workBack->next = newWork;
    pool->workBack = pool->workBack->next;
  }
}

void dequeue(tpool_t tpool, void** val, void (**callback) (tpool_t, void*)) {
  if (tpool->workFront != NULL) {
    *val = tpool->workFront->arg;
    *callback = tpool->workFront->thread_fun;
    work_t* work_free = tpool->workFront;
    work_t* new_front = tpool->workFront->next;
    tpool->workFront = new_front;

    if (work_free == tpool->workBack) {
      tpool->workBack = NULL;
    }
    work_free->next = NULL;
    free(work_free);
  } else {
    *val = NULL;
    if (callback)
      *callback = NULL;
  }
}


/* Function executed by each pool worker thread. This function is
 * responsible for running individual tasks. The function continues
 * running as long as either the pool is not yet joined, or there are
 * unstarted tasks to run. If there are no tasks to run, and the pool
 * has not yet been joined, the worker thread must be blocked.
 * 
 * Parameter: param: The pool associated to the thread.
 * Returns: nothing.
 */
static void *worker_thread(void *param) {
  tpool_t pool = param;

  /* TO BE COMPLETED BY THE STUDENT */

  while (1) {
    uthread_sem_wait(pool->tasks); // check if tasks remaining. If there are take one, if there aren't then wait.

    if (pool->joined) {
      break;
    }

    void* val = 0;
    void (*callback) (tpool_t, void*);

    uthread_sem_wait(pool->lock);
    dequeue (pool, &val, &callback);
    uthread_sem_signal(pool->lock);


    // if (!pool->tasks) {
    //   uthread_cond_signal(pool->noTasks);
    // }

    callback(pool, val);
    // uthread_mutex_unlock(pool->mx);
    
  }
  
  return NULL;
}

/* Creates (allocates) and initializes a new thread pool. Also creates
 * `num_threads` worker threads associated to the pool, so that
 * `num_threads` tasks can run in parallel at any given time.
 *
 * Parameter: num_threads: Number of worker threads to be created.
 * Returns: a pointer to the new thread pool object.
 */
tpool_t tpool_create(unsigned int num_threads) {

  /* TO BE COMPLETED BY THE STUDENT */

  tpool_t pool = malloc(sizeof(struct tpool));

  pool->threads     = malloc(sizeof(uthread_t) * num_threads);
  pool->tasks       = uthread_sem_create(0);
  pool->lock        = uthread_sem_create(1);
  pool->joined      = 0;
  pool->workFront   = NULL;
  pool->workBack    = NULL;
  pool->threadCount = num_threads;
  // TODO
  for (int i = 0; i < num_threads; i++) {
    pool->threads[i] = uthread_create(worker_thread, pool);
  }

  return pool;
}

/* Queues a new task, to be executed by one of the worker threads
 * associated to the pool. The task is represented by function `fun`,
 * which receives the pool and a generic pointer as parameters. If any
 * of the worker threads is available, `fun` is started immediately by
 * one of the worker threads. If all of the worker threads are busy,
 * `fun` is scheduled to be executed when a worker thread becomes
 * available. Tasks are retrieved by individual worker threads in the
 * order in which they are scheduled, though due to the nature of
 * concurrency they may not start exactly in the same order. This
 * function returns immediately, and does not wait for `fun` to
 * complete.
 *
 * Parameters: pool: the pool that is expected to run the task.
 *             fun: the function that should be executed.
 *             arg: the argument to be passed to fun.
 */
void tpool_schedule_task(tpool_t pool, void (*fun)(tpool_t, void *),
                         void *arg) {

  /* TO BE COMPLETED BY THE STUDENT */
  if (fun == NULL || arg == NULL)
    return;

  uthread_sem_wait(pool->lock);
  enqueue(pool, arg, fun);
  uthread_sem_signal(pool->lock);
  uthread_sem_signal(pool->tasks);
}

/* Blocks until the thread pool has no more scheduled tasks; then,
 * joins all worker threads, and frees the pool and all related
 * resources. Once this function returns, the pool cannot be used
 * anymore.
 *
 * Parameters: pool: the pool to be joined.
 */
void tpool_join(tpool_t pool) {

  /* TO BE COMPLETED BY THE STUDENT */
  // uthread_mutex_lock(pool->mx);
  //   while (pool->tasks) {
  //     uthread_cond_wait(pool->noTasks);
  //   }
  //   pool->joined = 1;
  //   uthread_cond_broadcast(pool->tasksRem);
  // uthread_mutex_unlock(pool->mx);


  //   // stack smash here?
  //   for (int i = 0; i < pool->threadCount; i++) {
  //     uthread_join (pool->threads[i], NULL);
  //   }


  //   work_t* tmp;

  //   while (pool->workFront) {
  //     tmp = pool->workFront;
  //     pool->workFront = pool->workFront->next;
  //     free(tmp);
  //   }

  //   free(pool->threads);
}
