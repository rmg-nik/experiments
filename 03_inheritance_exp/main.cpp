
#include <iostream>
#include <typeinfo>

using TypeId_t = std::uint64_t;

template<typename T>
TypeId_t get_unique_id()
{
    static TypeId_t hash = typeid(T).hash_code();
    std::cout << "name: " << typeid(T).name() << " hash: " << hash << " function: " << __FUNCSIG__ << "\n";
    return hash;
}

struct BaseClass
{
    virtual TypeId_t GetTypeId() const = 0;
};

template<typename T>
struct BaseClassHelper : public BaseClass
{
    TypeId_t GetTypeId() const final
    {
        static auto id = get_unique_id<T>();
        return id;
    }
};


struct MyClassA : public BaseClassHelper<MyClassA>
{
    void fooA()
    {
        std::cout << "name: " << typeid(*this).name() << ":" << a << "\n";
    }

    const std::int8_t a = 100;
};

struct MyClassB : public BaseClassHelper<MyClassB>
{
    void fooB()
    {
        std::cout << "name: " << typeid(*this).name() << ":" << b << "\n";
    }

    const int b = 200;
};

struct MyClassC : public BaseClassHelper<MyClassC>
{
    void fooC()
    {
        std::cout << "name: " << typeid(*this).name() << ":" << c << "\n";
    }

    const double c = 300.0;
};

int main(int, char**)
{
    MyClassA a;
    MyClassB b;
    BaseClass* c = new MyClassC;

    MyClassB* c_cast = static_cast<MyClassB*>(c);

    std::cout << a.GetTypeId() << "\n";
    std::cout << b.GetTypeId() << "\n";
    std::cout << c->GetTypeId() << "\n";
    std::cout << c_cast->GetTypeId() << "\n";
    c_cast->fooB();

    system("pause");
    return 0;
}