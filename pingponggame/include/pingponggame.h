#ifndef PINGPONGGAME_H
#define PINGPONGGAME_H

#include "game.h"
#include "pingpongball.h"
#include "gameevent.h"
#include <vector>
#include <queue>
#include <limits>
#include <iostream>

class PingPongGame : public Game
{
public:
    PingPongGame(std::string name, int nBall = 100, float baseRadius = 1.0f, float wantedFPS = 100.0f);
    virtual ~PingPongGame();

    void createGame(std::shared_ptr<Game> pGame);
    void loop();
protected:

    std::vector<ptrPingPongBall> m_balls;
    GameEventQueue m_eventQueue;
    float m_currentTime;
    int m_nBall;
    float m_baseRadius;
    float m_wantedFPS;
private:
    void step(float dt);
    void predict(ptrPingPongBall, float limit = std::numeric_limits<float>::infinity());
    void moveBalls(float dt);

    friend class Event;
};

struct Event : public GameEvent {

    std::weak_ptr<PingPongBall> pA, pB;
    int cA, cB;

    Event(float t, ptrPingPongBall a, ptrPingPongBall b);
    bool isValid() const;
    void getConsequences();
};

#endif // PINGPONGGAME_H
