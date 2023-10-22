#include <iostream>
#include <cstdlib>

struct refs
{
    virtual ~refs() {};
    virtual void foo() = 0;
};

class refs_impl : virtual public refs
{
public:
    ~refs_impl() override
    {

    }

    void foo() override
    {
        std::cout << "refs_impl " << ololo << "\n";
    }

private:
    int ololo{ 0 };
};

class IA : virtual public refs
{

};

class A : public IA, public refs_impl
{
public:
    ~A() override
    {

    }
};


int main(int, char**)
{
    IA* a = new A;

    A* aa = static_cast<A*>(a);

    aa->foo();

    std::system("pause");
    
    delete a;

    return 0;
}