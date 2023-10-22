#include <memory>

class Foo
{
public:

    Foo()
    {
        ++m_field;
    }

    ~Foo()
    {
        --m_field;
    }

    static Foo* Instance()
    {
        static std::unique_ptr<Foo> instance;
        if (!instance)
            instance.reset(new Foo);

        return instance.get();
    }

    int GetField()
    {
        return m_field;
    }

private:

    int m_field{ 0 };
};

class Bar
{
public:

    Bar()
    {
        ++m_field;
    }

    ~Bar()
    {
        auto field = Foo::Instance()->GetField();
        --m_field;
    }

    static Bar* Instance()
    {
        static std::unique_ptr<Bar> instance;
        if (!instance)
            instance.reset(new Bar);

        return instance.get();
    }

    int GetField()
    {
        return m_field;
    }

private:

    int m_field{ 0 };
};

int main(int, char**)
{
    Bar::Instance();
    Foo::Instance();
    return 0;
}