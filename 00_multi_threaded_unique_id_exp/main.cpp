#include <iostream>
#include <atomic>
#include <future>
#include <set>
#include <list>
#include <vector>
#include <cassert>

constexpr int ASYNC_COUNT = 10;
constexpr int EXP_COUNT = 100;

int get_unique_id()
{
    static std::atomic<int> counter(0);
    return ++counter;
}

int main(int, char**)
{

    std::list <std::future<std::vector<int>>> futures;

    for (int i = 0; i < ASYNC_COUNT; ++i)
    {
        futures.push_back(std::async([]()
            {
                std::vector<int> ret(EXP_COUNT);
                for (int j = 0; j < EXP_COUNT; ++j)
                    ret[j] = get_unique_id();
                return ret;
            }));
    }

    std::set<int> check_set;
    for (auto& f : futures)
    {
        auto vec = f.get();
        check_set.insert(vec.begin(), vec.end());
    }
    std::cout << check_set.size() << "\n";
    system("pause");
    return 0;
}