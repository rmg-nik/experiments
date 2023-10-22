#include <thread>
#include <mutex>
#include <deque>
#include <functional>
#include <unordered_map>
#include <memory>

template<typename T>
class TSQueue
{
public:

    void PushBack(T&& val)
    {
        std::unique_lock<std::mutex> lock(m_access_storage_mutex);
        m_storage.push_back(val);
    }

    bool PopFront(T& val)
    {
        std::unique_lock<std::mutex> lock(m_access_storage_mutex);
        if (m_storage.empty())
            return false;

        val = std::move(m_storage.front());
        m_storage.pop_front();
        return true;
    }

private:

    std::deque<T> m_storage;
    std::mutex    m_access_storage_mutex;
};


class IPlayer
{
public:

    using PlayerId_t = std::int32_t;

    virtual PlayerId_t GetId() const = 0;
    virtual void ProcessMessage(const std::string&) = 0;
};

using PlayerPtr_t = std::shared_ptr<IPlayer>;

class IRoom
{
    using RoomId_t = std::int32_t;
    virtual RoomId_t GetId() const = 0;
    virtual void AddPlayer(const PlayerPtr_t& player) = 0;
    virtual void RemovePlayer(IPlayer::PlayerId_t id) = 0;
    virtual void BoadcastMessage(const std::string&) = 0;
};

class Manager;
using ManagerWeakPtr_t = std::shared_ptr<Manager>;

class Player : public IPlayer
{
public:

    Player(PlayerId_t id, ManagerWeakPtr_t manager) 
        : m_id{ id }
        , m_manager(manager)
    {

    }

    PlayerId_t GetId() const override
    {

    }

private:

    void WorkingThread()
    {

    }

private:

    const PlayerId_t    m_id{ -1 };
    ManagerWeakPtr_t    m_manager;
};

class Manager
{
public:

    void SendMessage(const IPlayer::PlayerId_t player_id, const std::string& msg);

private:

    using PlayersMap_t = std::unordered_map<IPlayer::PlayerId_t, PlayerPtr_t>;

    PlayersMap_t    m_players;
    std::mutex      m_access_players_mutex;
};

class PlayerConstructor
{
public:
    static PlayerPtr_t CreatePlayer(ManagerWeakPtr_t manager)
    {
        static IPlayer::PlayerId_t id = 0;
        return std::make_shared<IPlayer>(++id, manager);
    }
};

int main(int, char**)
{
    return 0;
}