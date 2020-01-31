#ifndef PINGPONGGAME_H
#define PINGPONGGAME_H

#include "game.h"
#include "pingpongball.h"
#include <vector>
#include <queue>
#include <limits>
//#include <iostream>

struct Event {
    float m_t;
    std::weak_ptr<PingPongBall> pA, pB;
    int cA, cB;
    Event(float t, ptrPingPongBall a, ptrPingPongBall b) : m_t(t), pA(a), pB(b) {
        cA = a ? a->getCount() : -1;
        cB = b ? b->getCount() : -1;
//        std::cout << t << " " << a << " " << b << std::endl;
    }

    bool isValid() const {
        auto a = pA.lock(), b = pB.lock();
        if (a && a->getCount() != cA) return false;
        if (b && b->getCount() != cB) return false;
        return true;
    }
};

bool operator< (const Event& a, const Event& b);

class PingPongGame : public Game
{
    public:
        PingPongGame(std::string name, int nBall = 100, float baseRadius = 1.0f);
        virtual ~PingPongGame();

        void createGame(std::shared_ptr<Game> pGame);
        void loop();
    protected:

        std::vector<ptrPingPongBall> m_balls;
        std::priority_queue<Event> m_eventQueue;
        float m_currentTime;
        int m_nBall;
        float m_baseRadius;
    private:
        void step(float dt);
        void predict(ptrPingPongBall, float limit = std::numeric_limits<float>::infinity());
        void moveBalls(float dt);
};

#endif // PINGPONGGAME_H
