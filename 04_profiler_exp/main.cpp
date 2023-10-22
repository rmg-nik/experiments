#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <vector>
#include <functional>
#include <deque>
#include <cmath>
#include <iostream>
#include <string>

class Worker
{
public:

    using Task_t = std::function<void()>;

    Worker(size_t thread_count)
    {
        try
        {
            m_is_working = true;
            thread_count = std::max(thread_count, static_cast<size_t>(1));
            m_worker_threads.reserve(thread_count);
            for (size_t i = 0; i < thread_count; ++i)
            {
                m_worker_threads.push_back(std::thread([this]() { WorkingThread(); }));
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        catch (...)
        {
            Cleanup();
            throw;
        }
    }

    void AddTask(Task_t&& task)
    {
        std::unique_lock<std::mutex> lock(m_worker_mutex);
        m_tasks.push_back(std::move(task));
        m_cv.notify_one();
    }

    ~Worker() noexcept
    {
        Cleanup();
    }

    bool HasTasks()
    {
        std::unique_lock<std::mutex> lock(m_worker_mutex);
        return !m_tasks.empty();
    }

private:

    void WorkingThread()
    {
        while (m_is_working)
        {
            Task_t task;
            {
                std::unique_lock<std::mutex> lock(m_worker_mutex);
                if (m_tasks.empty())
                    m_cv.wait(lock);

                if (m_tasks.empty())
                    continue;

                task = std::move(m_tasks.front());
                m_tasks.pop_front();
            }
            task();
        }
    }

    void Cleanup() noexcept
    {
        {
            std::unique_lock<std::mutex> lock(m_worker_mutex);
            m_is_working = false;
            m_tasks.clear();
            m_cv.notify_all();
        }
        for (auto& thread : m_worker_threads)
        {
            if (thread.joinable())
                thread.join();
        }
    }

private:

    std::vector<std::thread> m_worker_threads;
    std::condition_variable  m_cv;
    std::mutex               m_worker_mutex;
    std::atomic_bool         m_is_working{ false };
    std::deque<Task_t>       m_tasks;
};

std::mutex COUT_LOCK;

void f1_sum_rand(int id, int exp_count)
{
    double result{ 0. };
    for (int i = 0; i < exp_count; ++i)
    {
        result += (rand() % 100) / 100.;
    }
    std::unique_lock<std::mutex> lock(COUT_LOCK);
    std::cout << "\n f1_sum[id:" << id <<  "] = " << result;
}

void f2_sum_sin(int id, int exp_count)
{
    float result{ 0. };
    for (int i = 0; i < exp_count; ++i)
    {
        result += sin(i);
    }
    std::unique_lock<std::mutex> lock(COUT_LOCK);
    std::cout << "\n f2_sum_sin[id:" << id << "] = " << result;
}

int main(int, char**)
{
    {
        Worker worker(5);
        for (int i = 0; i < 300; ++i)
        {
            constexpr int EXP_COUNT = 10000;
            worker.AddTask(std::bind(f1_sum_rand, i, EXP_COUNT));
            worker.AddTask(std::bind(f2_sum_sin, i, EXP_COUNT));
        }

        while (worker.HasTasks())
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    //system("pause");
    return 0;
}