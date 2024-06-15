#include <functional>

class Object
{
};

class Context
{
public:
  template <typename taMessageType, typename taF, typename... taArgs>
  void SubscribeForMessages(const Object* aListener, taF&& aCallback, taArgs&&... aArgs)
  {
    SubscribeForMessages<taMessageType>(
        nullptr, aListener, std::forward<taF>(aCallback), std::forward<taArgs>(aArgs)...);
  }

  template <typename taMessageType, typename taF, typename... taArgs>
  void SubscribeForMessages(const Object* aSource, const Object* aListener, taF&& aCallback,
                            taArgs&&... aArgs)
  {
    std::function<void(const taMessageType&)> func = std::bind(
        std::forward<taF>(aCallback), std::forward<taArgs>(aArgs)..., std::placeholders::_1);
  }
};

class MessageA
{
};

class Test : public Object
{
public:
  Test()
  {
    Context context;
    context.SubscribeForMessages<MessageA>(this, &Test::OnMessage, this);
    context.SubscribeForMessages<MessageA>(static_cast<const Object*>(nullptr),
                                           static_cast<const Object*>(this),
                                           &Test::OnMessage,
                                           this);
    // Next line instantiate SubscribeForMessages(const Object*, taF&&, taArgs&&...) instead of
    // SubscribeForMessages(const Object*, const Object*, taF&&, taArgs&&...)
    // context.SubscribeForMessages<MessageA>(nullptr, this, &Test::OnMessage, this);
  }

  void OnMessage(const MessageA& msg)
  {
  }
};

int main()
{
  Test test;
}
