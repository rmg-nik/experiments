#include <cstdlib>
#include <iostream>
#include <string>
#include <typeinfo>

using Id = std::uint64_t;

Id HelperGetUniqueId()
{
    static Id counter = 0;
    return ++counter;
}

template <typename T>
const std::string& HelperGetTypeName()
{
    const static std::string type_name = typeid(T).name();
    return type_name;
}

template <typename T>
Id HelperGetTypeId()
{
    static Id type_id = std::hash<std::string>{}(HelperGetTypeName<T>());
    return type_id;
}

struct TypeInfo
{
    virtual Id GetTypeId() const = 0;

    virtual const std::string& GetTypeName() const = 0;

    virtual ~TypeInfo(){};
};

template <typename Parent, typename Child>
class TypeInfoHelper : public Parent
{
  public:
    using Parent::Parent;

    static Id TypeId()
    {
        return m_type_id;
    }

    static const std::string& TypeName()
    {
        return m_type_name;
    }

    Id GetTypeId() const override
    {
        return TypeId();
    }

    const std::string& GetTypeName() const override
    {
        return TypeName();
    }

  private:
    // for debug purpose type_id and type_name are members
    static const Id m_type_id;

    static const std::string m_type_name;
};

template <typename Parent, typename Child>
const Id TypeInfoHelper<Parent, Child>::m_type_id = HelperGetTypeId<Child>();

template <typename Parent, typename Child>
const std::string TypeInfoHelper<Parent, Child>::m_type_name = HelperGetTypeName<Child>();

class Object : public TypeInfo
{
};

class ObjectA : public TypeInfoHelper<Object, ObjectA>
{
};

class ObjectB : public TypeInfoHelper<ObjectA, ObjectB>
{
};

int main(int, char**)
{

    {
        ObjectA obj;
        std::cout << "ObjectA: " << obj.GetTypeName() << std::endl;
    }
    {
        ObjectB obj;
        std::cout << "ObjectB: " << obj.GetTypeName() << std::endl;
    }
    {
        Object* obj = new ObjectA;
        std::cout << "Object* objAPtr = new ObjectA: " << obj->GetTypeName() << std::endl;
    }
    {
        Object* obj = new ObjectB;
        std::cout << "Object* objAPtr = new ObjectB: " << obj->GetTypeName() << std::endl;
    }
    {
        ObjectA* obj = new ObjectA;
        std::cout << "ObjectA* obj = new ObjectA: " << obj->GetTypeName() << std::endl;
    }
    {
        ObjectA* obj = new ObjectB;
        std::cout << "ObjectA* obj = new ObjectB: " << obj->GetTypeName() << std::endl;
    }
    {
        ObjectB* obj = new ObjectB;
        std::cout << "ObjectB* obj = new ObjectB: " << obj->GetTypeName() << std::endl;
    }
    std::system("pause");

    return 0;
}