#ifndef ROCK_H
#define ROCK_H

#include "GameObject.h"


class Rock : public GameObject
{
    public:
        Rock(std::shared_ptr<Game> pGame);
        virtual ~Rock();

        void setSize(float size) { setSize(size, size); }
        void setSize(float width, float height);
        void setPosition(float x, float y);
        void setColor(sf::Color color);
        void startClock() { m_clockStarted = true; m_clock.restart(); }

        void draw(sf::RenderWindow& window) const;
        void printStatus() const;
    protected:
        float m_width, m_height;
        float m_x, m_y;
        float m_lastx, m_lasty;
        float m_vx, m_vy;
        bool m_clockStarted = false;
        sf::Clock m_clock;
        sf::RectangleShape m_shape;

        void computeVelocity();
    private:
};

typedef std::shared_ptr<Rock> ptrRock;

#endif // ROCK_H
