#include <pthread.h>
#include <functional>
#include <queue>
#include <vector>

class Task {
 private:
  void (*m_f)(void*);
  void* m_args;

 public:
  Task(void (*f)(void*), void* args) : m_f(f), m_args(args){};
  ~Task(){};
  void run() { (*m_f)(m_args); };
};

enum PoolState { STARTED, STOPPED };

class PoolThread {
 private:
  PoolThread(const int poolSize) : m_poolSize(poolSize){};

  int m_poolSize;
  std::vector<pthread_t> m_threads;
  pthread_mutex_t m_taskMutex;
  pthread_cond_t m_taskCond;
  std::queue<Task*> m_tasks;
  PoolState m_poolState;

 public:
  ~PoolThread();

  void init();
  void destroy();
  void wait(bool unlock = true);
  void executeThread();
  void addTask(Task* task);
  void addTasks(std::vector<Task*> tasks);

  static PoolThread& getInstance(const int poolSize);
  void operator=(PoolThread const&) = delete;
};
