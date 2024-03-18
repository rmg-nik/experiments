#include <iostream>
#include <type_traits>
#include <vector>

template <typename T>
struct has_is_valid_data
{
    using Yes = std::int8_t;
    using No = std::int16_t;
    template <typename U>
    static Yes check(std::enable_if_t<std::is_member_function_pointer_v<decltype(&U::IsDataValid)>,
                                      bool> = true);

    template <typename U>
    static No         check(...);
    static bool const value = sizeof(check<T>(true)) == sizeof(Yes);
};

struct DataOld
{
    int data{0};

    bool IsValid() const
    {
        return data > 0;
    }
};

struct DataNew
{
    int data{0};
};

struct DataProcessorOld
{
};

struct DataProcessorNew
{
    bool IsDataValid(const DataNew& data)
    {
        return data.data > 0;
    }
};

template <typename taDataProcessor, typename taData>
struct DataStorage
{
    template <typename taT = taDataProcessor,
              typename std::enable_if_t<has_is_valid_data<taT>::value, bool> = true>
    void RemoveInvalidData(std::vector<taData>& data)
    {
        data.erase(
            std::remove_if(data.begin(),
                           data.end(),
                           [this](const auto& val) { return m_data_processor.IsDataValid(val); }),
            data.end());
    }

    template <typename taT = taDataProcessor,
              typename std::enable_if_t<!has_is_valid_data<taT>::value, bool> = true>
    void RemoveInvalidData(std::vector<taData>& data)
    {
        data.erase(
            std::remove_if(data.begin(), data.end(), [](const auto& val) { return val.IsValid(); }),
            data.end());
    }

    taDataProcessor m_data_processor;
};

template <typename taDataProcessor, typename taData>
struct DataStorageCpp20
{
    void RemoveInvalidData(std::vector<taData>& data)
    {
        if constexpr (has_is_valid_data<taDataProcessor>::value)
        {
            data.erase(std::remove_if(
                           data.begin(),
                           data.end(),
                           [this](const auto& val) { return m_data_processor.IsDataValid(val); }),
                       data.end());
        }
        else
        {
            data.erase(std::remove_if(
                           data.begin(), data.end(), [](const auto& val) { return val.IsValid(); }),
                       data.end());
        }
    }

    taDataProcessor m_data_processor;
};

int main(int, char**)
{
    std::vector<DataOld> data_old;
    std::vector<DataNew> data_new;

    DataStorage<DataProcessorOld, DataOld> processor_old;
    DataStorage<DataProcessorNew, DataNew> processor_new;
    processor_old.RemoveInvalidData(data_old);
    processor_new.RemoveInvalidData(data_new);

    DataStorageCpp20<DataProcessorOld, DataOld> processor_cpp20_old;
    DataStorageCpp20<DataProcessorNew, DataNew> processor_cpp20_new;
    processor_cpp20_old.RemoveInvalidData(data_old);
    processor_cpp20_new.RemoveInvalidData(data_new);

    return 0;
}
