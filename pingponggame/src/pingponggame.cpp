#include "pingponggame.h"
#include <iostream>
#include <random>
#include <cassert>

using namespace std;

PingPongGame::PingPongGame(std::string name, int nBall, float baseRadius)
    : Game(name), m_currentTime(0.f), m_nBall(nBall), m_baseRadius(baseRadius)
{

}

PingPongGame::~PingPongGame()
{
    //dtor
}

void PingPongGame::createGame(std::shared_ptr<Game> pGame)
{
    default_random_engine generator(42);
    uniform_real_distribution<float> distribution(0.0f, 1.0f);
    uniform_int_distribution<int> color_distribution(128, 255);
    uniform_real_distribution<float> velocity_distribution(-0.1f, 0.1f);
    uniform_real_distribution<float> radius_distribution(m_baseRadius / 2 / m_nBall, m_baseRadius * 2 / m_nBall);

    for (int i = 0; i < m_nBall; i++) {
        ptrPingPongBall ball = make_shared<PingPongBall>(pGame);
        ball->setRadius(radius_distribution(generator));
        ball->setPosition(distribution(generator), distribution(generator));
        ball->setMass(ball->getRadius()*100.0f);
        ball->setColor(color_distribution(generator), color_distribution(generator), color_distribution(generator));
        ball->setVelocity(velocity_distribution(generator), velocity_distribution(generator));

        m_balls.push_back(ball);
        pGame->registerObject(ball);
    }

    for (auto ball : m_balls) predict(ball);
    cout << "Event queue prepared " << m_eventQueue.size() << endl;
}

void PingPongGame::loop()
{
    float oldTime = m_currentTime;
    int n = 0;
    float elapsed = 0.f;
    float wantedFPS = 100.0f;
    float timeIncrement = 1.0 / wantedFPS;
    while (m_window.isOpen())
    {
        sf::Clock clock;
        //this->printStatus();
        sf::Event event;
        while (m_window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                m_window.close();
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            //sf::Vector2i localPosition = sf::Mouse::getPosition(m_window);
            //m_pBall->setPosition(localPosition.x, localPosition.y);
        }

        step(timeIncrement);
        renderGame();

        float currentLoopTime = clock.getElapsedTime().asSeconds();
        if (currentLoopTime < timeIncrement) elapsed += timeIncrement;
        else elapsed += currentLoopTime;

        n++;
//        cout << "queue " << m_eventQueue.size() << endl;
        if (m_currentTime > oldTime + 1.f) {
            oldTime = m_currentTime;
            cout << "check queue " << m_eventQueue.size() << " real fps " << (float(n)/elapsed) << " wantedFPS " << wantedFPS << endl;
            n = 0, elapsed = 0.f;
//            break;
        }
        while (clock.getElapsedTime().asSeconds() < timeIncrement) { }
    }
    cout << "Done loop" << endl;
}

void PingPongGame::step(float dt)
{
    float nextTime = m_currentTime + dt;

    while (!m_eventQueue.empty()) {
        Event e = m_eventQueue.top();
        if (!e.isValid()) {
//            cout << "event invalid " << e.m_t << " " << m_eventQueue.size() << endl;
            m_eventQueue.pop();
        } else {
            if (e.m_t > nextTime) { // event is too far in future, move all particles to nextTime
                break; // get out of the loop to render
            } else { // event happens inside [m_currentTime, nextTime]
//                cout << "event " << e.m_t << " " << m_currentTime << " " << nextTime << " " << m_eventQueue.size() << endl;
                moveBalls(e.m_t-m_currentTime);

                ptrPingPongBall pA = e.pA.lock(), pB = e.pB.lock();
                if (pA && pB) pA->bounceOff(pB);
                else if (pA && !pB) pA->bounceOffVerticalWall();
                else if (!pA && pB) pB->bounceOffHorizontalWall();
                else {}
                m_eventQueue.pop(); // remove this event to process next event
                predict(pA);
                predict(pB);
            }
        }
    }

    if (m_currentTime < nextTime) moveBalls(nextTime-m_currentTime);
}

bool operator< (const Event& a, const Event& b) {
    return a.m_t > b.m_t;
}

void PingPongGame::predict(ptrPingPongBall pBall, float limit)
{
    if (!pBall) return;

    for (auto pThat : m_balls) {

        float dt = pBall->getTimeToHit(pThat);
        if (dt > 0 && m_currentTime+dt < limit) {
            m_eventQueue.push(Event(m_currentTime+dt, pBall, pThat));
            m_eventQueue.push(Event(m_currentTime+dt, pThat, pBall));
        }

    }

    double dtX = pBall->getTimeToHitVerticalWall();
    double dtY = pBall->getTimeToHitHorizontalWall();
    if (dtX > 0 && m_currentTime + dtX < limit) m_eventQueue.push(Event(m_currentTime + dtX, pBall, nullptr));
    if (dtY > 0 && m_currentTime + dtY < limit) m_eventQueue.push(Event(m_currentTime + dtY, nullptr, pBall));
}

void PingPongGame::moveBalls(float dt)
{
    for (auto ball : m_balls) {
        pair<sf::Vector2f, sf::Vector2f> pos_velo = ball->getPredictedPosition(dt);
        sf::Vector2f pos = pos_velo.first, velo = pos_velo.second;
        ball->setPosition(pos.x, pos.y);
        ball->setVelocity(velo.x, velo.y);
//        ball->printStatus();
    }
    m_currentTime += dt;
}
