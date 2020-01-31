#ifndef GAME_EVENT_H
#define GAME_EVENT_H

#include <memory>
#include <queue>

class GameEvent
{
    public:
        GameEvent(double t);
        virtual ~GameEvent();

        double getTime() const { return m_t; }
        virtual void getConsequences() {}
        virtual bool isValid() const { return false; }

        friend bool operator <(const GameEvent& e1, const GameEvent& e2);
        friend bool operator <(const std::shared_ptr<GameEvent>& p1, const std::shared_ptr<GameEvent>& p2);
    protected:
        double m_t;

    private:

};

class GameEventComparison {
public:
    bool operator() (const std::shared_ptr<GameEvent>& p1, const std::shared_ptr<GameEvent>& p2) const {
        return p1->getTime() > p2->getTime();
    }
};

typedef std::priority_queue<std::shared_ptr<GameEvent>, std::vector<std::shared_ptr<GameEvent>>, GameEventComparison > GameEventQueue;

#endif // EVENT_H
