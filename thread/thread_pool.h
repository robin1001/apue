/* Created on 2016-05-24
 * Author: Binbin Zhang
 */
#ifndef THREAD_POOL_H_
#define THREAD_POOL_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <unistd.h>
#include <pthread.h>

#include <vector>
#include <queue>

void ErrorExit(const char *msg) {
    perror(msg);
    exit(1);
}

// Thread interface for thread class
class Threadable {
public:
    virtual void operator() () = 0;
    virtual ~Threadable() {}
    static void *Run(void *in) {
        Threadable *t = static_cast<Threadable *>(in);
        (*t)();
        return NULL;
    }
};

// ThreadPool
class ThreadPool {
public:
    ThreadPool(int num_thread = 1): num_thread_(num_thread), stop_(false) {
        if (pthread_mutex_init(&mutex_, NULL) != 0) {
            ErrorExit("mutex init error");
        }
        if (pthread_cond_init(&cond_, NULL) != 0) {
            ErrorExit("cond init error");
        }
        // Create num_thread thread at once
        threads_.resize(num_thread_);
        for (int i = 0; i < threads_.size(); i++) {
            if (pthread_create(&threads_[i], NULL, 
                               ThreadPool::WorkerThread, (void *)this) != 0) {
                ErrorExit("pthread create error");
            }
        }
    }

    ~ThreadPool() {
        pthread_mutex_lock(&mutex_);
        stop_ = true;
        pthread_mutex_unlock(&mutex_);
        // notify all thread to stop
        pthread_cond_broadcast(&cond_);

        for (int i = 0; i < threads_.size(); i++) {
            pthread_join(threads_[i], NULL);
        }

        pthread_mutex_destroy(&mutex_);
        pthread_cond_destroy(&cond_);
    }

    void AddTask(Threadable *task) {
        pthread_mutex_lock(&mutex_);
        task_queue_.push(task);
        pthread_mutex_unlock(&mutex_);
        pthread_cond_signal(&cond_);
    }

    // Wait a task to execute  
    Threadable *WaitTask() {
        Threadable *task = NULL;
        pthread_mutex_lock(&mutex_);
        while (!stop_ && task_queue_.empty()) {
            pthread_cond_wait(&cond_, &mutex_);
        }
        if (task_queue_.size() > 0) {
            task = task_queue_.front();
            task_queue_.pop();
        }
        // else stop_ = true return NULL
        pthread_mutex_unlock(&mutex_);
        return task;
    }

    // PoolWorker thread
    static void *WorkerThread(void *arg) {
        ThreadPool *pool = static_cast<ThreadPool *>(arg);
        for(;;) {
            Threadable *task = pool->WaitTask();
            // Stop
            if (task == NULL) break;
            else {
                (*task)(); // Run the task
                delete task;
            }
        }
    }

private:
    int num_thread_;
    bool stop_;
    std::vector<pthread_t> threads_;
    std::queue<Threadable *> task_queue_; //TaskQueue
    pthread_cond_t cond_;
    pthread_mutex_t mutex_;
};

#endif
