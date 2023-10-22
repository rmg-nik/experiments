#include <iostream>
#include <map>
#include <unordered_map>

std::string get_ranom_string(size_t len)
{
    std::string ret(len, '\0');
    for (size_t i = 0; i < len; ++i)
    {
        char c = (char)(rand() % (int)('Z' - 'a') + 'a');
        ret[i] = c;
    }
    return ret;
}

int main(int, char**)
{
    std::unordered_map<std::string, std::string> exp;

    for (int i = 0; i < 300; ++i)
    {
        exp[get_ranom_string(10)] = get_ranom_string(10);
    }

    for (auto it = exp.begin(); it != exp.end();)
    {
        it = exp.erase(it);
    }
    system("pause");
    return 0;
}