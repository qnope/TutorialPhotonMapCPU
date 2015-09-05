#ifndef THREADPOOL_HPP
#define THREADPOOL_HPP

#include <mutex>
#include <condition_variable>
#include <thread>
#include <functional>
#include <atomic>
#include <vector>
#include <deque>

class ThreadPool {
    class Worker {
    public :
        Worker(ThreadPool &pool) : mPool(pool) {}

        void operator()() {
            std::function<void()> task;

            while(true) {
                {
                    std::unique_lock<std::mutex> lock(mPool.mTaskMutex);

                    while(!mPool.mStop && mPool.mTasks.empty())
                        mPool.mCondition.wait(lock);

                    if(mPool.mStop)
                        return;

                    task = mPool.mTasks.front();
                    mPool.mTasks.pop_front();
                }

                task();
            }
        }

    private:
        ThreadPool &mPool;
    };

public:
    ThreadPool(std::size_t numberThreads) {
        mStop = false;

        for(auto i(0u); i < numberThreads; ++i)
            mWorkers.emplace_back(std::thread(Worker(*this)));
    }

    template<typename Callable>
    void addTask(Callable &&callable) {
        {
            std::lock_guard<std::mutex> lock(mTaskMutex);
            mTasks.push_back(std::function<void()>(callable));
        }
        mCondition.notify_one();
    }

    ~ThreadPool() {
        {
            std::lock_guard<std::mutex> lock(mTaskMutex);
            mStop = true;
        }

        mCondition.notify_all();

        for(auto &thread : mWorkers)
            thread.join();
    }

private:
    std::vector<std::thread> mWorkers;
    std::deque<std::function<void()>> mTasks;
    std::mutex mTaskMutex;
    std::condition_variable mCondition;
    std::atomic<bool> mStop;
};

#endif // THREADPOOL_HPP

