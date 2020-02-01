#include "Rock.h"
#include <iostream>
using namespace std;

Rock::Rock(shared_ptr<Game> pGame)
    : GameObject(pGame)
{
    setSize(0.1f, 0.1f);
    setPosition(0.5f, 0.5f);
}

Rock::~Rock()
{
    //dtor
}

void Rock::setSize(float width, float height)
{
    m_width = width;
    m_height = height;

    width *= getGame()->getWindow().getSize().x;
    height *= getGame()->getWindow().getSize().y;
    m_shape.setSize(sf::Vector2f(width, height));
    m_shape.setOrigin(width/2, height/2);
}

void Rock::setPosition(float x, float y)
{
    m_lastx = m_x, m_lasty = m_y;
    m_x = x, m_y = y;
    computeVelocity();
    x *= getGame()->getWindow().getSize().x;
    y *= getGame()->getWindow().getSize().y;
    m_shape.setPosition(x, y);
}

void Rock::computeVelocity()
{
    if (m_clockStarted) {
        float dt = m_clock.restart().asSeconds();
        m_vx = (m_x-m_lastx) / dt;
        m_vy = (m_y-m_lasty) / dt;
    } else {
        m_vx = 0;
        m_vy = 0;
    }
}

void Rock::setColor(sf::Color color)
{
    m_shape.setFillColor(color);
}

void Rock::draw(sf::RenderWindow& window) const
{
    window.draw(m_shape);
}

void Rock::printStatus() const
{
    cout << "rock " << m_id << endl;
}
