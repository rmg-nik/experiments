#include <thread>
#include <mutex>
#include <vector>
#include <memory>
#include <chrono>
#include <random>

constexpr const int THREAD_COUNT = 500;
constexpr const int EXP_COUNT = 100000000;
constexpr const int VECTORS_COUNT = 100;
constexpr const int VECTOR_MAX_CAPACITY = 1000;

int* global_data[VECTORS_COUNT];
std::mutex lock_global_data;

int main(int, char**)
{
    for (auto& d : global_data)
        d = nullptr;

    std::vector<std::unique_ptr<std::thread>> exp_threads;
    for (int i = 0; i < THREAD_COUNT; ++i)
    {
        exp_threads.push_back(std::make_unique<std::thread>([]()
            {
                std::string random_string;
                std::vector<std::int64_t> vectors[VECTORS_COUNT];
                int from_global = 0;
                for (int exp = 0; exp < EXP_COUNT; ++exp)
                {
                    std::this_thread::yield();
                    random_string = "";

                    int id = rand() % VECTORS_COUNT;
                    vectors[id].clear();
                    vectors[id].shrink_to_fit();
                    int new_size = rand() % VECTOR_MAX_CAPACITY + 1;

                    if (exp % 3)
                    {
                        std::lock_guard<std::mutex> lock(lock_global_data);
                        if (global_data[id])
                            delete[] global_data[id];
                        global_data[id] = new int[new_size];
                        from_global = global_data[id][new_size / 2];
                    }
                    for (int j = 0; j < new_size; ++j)
                    {
                        std::string local_s = std::string(j + 1, (char)((exp + j) % 100 + 32));
                        random_string.push_back((char)((exp + j) % 100 + 32));
                        vectors[id].push_back(random_string[0] + local_s[0] + from_global);
                        vectors[id].push_back(local_s[0]);
                        local_s = random_string;
                    }
                    (void)random_string;
                    std::this_thread::sleep_for(std::chrono::microseconds(10));                    
                }
            }));
    }
    for (auto& t : exp_threads)
    {
        if (t->joinable())
            t->join();
    }

    for (auto& d : global_data)
        delete[] d;
    return 0;
}