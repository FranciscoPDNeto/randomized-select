#include <iostream>
#include "PoolThread.h"

void *startThread(void *arg) {
  PoolThread *poolThread = (PoolThread *) arg;
  poolThread->executeThread();

  return NULL;
}

PoolThread& PoolThread::getInstance(const int poolSize) {
  static PoolThread instance(poolSize);
  return instance;
}

PoolThread::~PoolThread() {
  if (m_poolState != PoolState::STOPPED)
    destroy();
}

void PoolThread::init() {
  m_poolState = PoolState::STARTED;
  pthread_mutex_init(&m_taskMutex, NULL);
	pthread_cond_init(&m_taskCond, NULL);

  for (int i = 0; i < m_poolSize; i++) {
    pthread_t thread;
    if (pthread_create(&thread, NULL, startThread, this) != 0) {
      std::cout << "The Thread could not be created.";
      return;
    }
    m_threads.push_back(thread);
  }

}

void PoolThread::addTask(Task *task) {
  pthread_mutex_lock(&m_taskMutex);
  m_tasks.push(task);
  pthread_cond_signal(&m_taskCond);
  pthread_mutex_unlock(&m_taskMutex);
}

void PoolThread::addTasks(std::vector<Task *> tasks) {
  for (const auto& task : tasks)
    addTask(task);
}

void PoolThread::wait(bool unlock) {
  pthread_mutex_lock(&m_taskMutex);
  while (m_poolState != PoolState::STOPPED && m_tasks.empty()) {
    pthread_cond_wait(&m_taskCond, &m_taskMutex);
  }
  if (unlock)
    pthread_mutex_unlock(&m_taskMutex);
}

void PoolThread::executeThread() {
  while (m_poolState != PoolState::STOPPED) {
    wait(false);

    if (m_poolState == PoolState::STOPPED) {
      pthread_mutex_unlock(&m_taskMutex);
      pthread_exit(NULL);
    }

    Task *task = m_tasks.front();
    m_tasks.pop();
    pthread_mutex_unlock(&m_taskMutex);
    task->run();
    delete task;
  }
}

void PoolThread::destroy() {
  pthread_mutex_lock(&m_taskMutex);
  m_poolState = PoolState::STOPPED;
  pthread_mutex_unlock(&m_taskMutex);
  pthread_cond_broadcast(&m_taskCond);

  for (int i = 0; i < m_poolSize; i++) {
    void *threadReturn;
    pthread_join(m_threads[i], &threadReturn);
  }
  while (!m_tasks.empty()) {
    Task *task = m_tasks.front();
    m_tasks.pop();
    delete task;
  }
}