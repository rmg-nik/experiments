#include <iostream>
#include <cstdint>
#include <unordered_map>
#include <functional>

using TypeId = std::size_t;

std::size_t unique_counter()
{
    static std::size_t counter = 0;
    return ++counter;
}

template <typename T>
struct TypeInfo
{
    static TypeId GetTypeId()
    {
        static TypeId type_id = unique_types_counter();
        //static TypeId type_id = typeid(T).hash_code();
        return type_id;
    }
};

struct Message
{
    virtual TypeId GetId() = 0;
    virtual ~Message() = default;
};

template<typename Derrived>
struct MessageTypeHelper : public Message
{
    TypeId GetId() override
    {
        return Id();
    }

    static TypeId Id()
    {
        return TypeInfo<Derrived>::GetTypeId();
    }
};

struct AppMessage : public MessageTypeHelper<AppMessage>
{
    enum class Event
    {
        Open,
        Close
    };

    Event m_event{};

    AppMessage(Event event) : m_event{ event } {};
};

struct TickMessage : public MessageTypeHelper<TickMessage>
{
    enum class Event
    {
        Tick
    };

    Event m_event{};

    TickMessage(Event event) : m_event{ event } {};
};

class MessageBus
{
public:

    template<typename MessageType>
    bool SubscribeToMessageType(std::function<void(const MessageType&)>&& handler);

    bool SubscribeToSource(void* source, std::function<void(const MessageType&)>&& handler);

    template<typename MessageType>
    bool SubscribeToEventAndSource(void* source);

    template<typename MessageType>
    void SendMessage(void* source, const MessageType& message);
};

template <typename MessageType, typename EventType>
bool IsEventOf(const Message& msg, EventType event)
{
    return msg.GetId() == Message<MessageType>::Id()
        && static_cast<const MessageType&>(msg).m_event == event;
}

void TestEvent(const Message& message)
{
   // if (IsEventOf<AppMessage::Event::Open>(message))
   // {
   //     std::cout << "event is AppMessage::Event::Open" << std::endl;
   // }
   // else
   // {
   //     std::cout << "unknown event" << std::endl;
   // }
}

int main(int, char**)
{
    AppMessage open_event{ AppMessage::Event::Open };
    TickMessage tick_event{ TickMessage::Event::Tick};
    TestEvent(open_event);
    TestEvent(tick_event);
    return 0;
}