#include "pingpongball.h"
#include <iostream>
#include <cmath>
using namespace std;

PingPongBall::PingPongBall(std::shared_ptr<Game> pGame)
    : GameObject(pGame), m_x(0.f), m_y(0.f), m_r(0.02f), m_vx(0.f), m_vy(0.f), m_mass(0.5f), m_count(0)
{
    setColor(0, 255, 0);
    setRadius(m_r);
    setPosition(m_x, m_y);
}

PingPongBall::~PingPongBall()
{
    //dtor
}

void PingPongBall::draw(sf::RenderWindow& window) const
{
    window.draw(m_shape);
}

void PingPongBall::setPosition(float x, float y)
{
    m_x = x, m_y = y;

    x *= getGame()->getWindow().getSize().x, y *= getGame()->getWindow().getSize().y;

    //cout << "x " << x << " y " << y << endl;
    m_shape.setPosition(x, y);
}

void PingPongBall::setRadius(float r)
{
    m_r = r;
    float radius = m_r*getGame()->getWindow().getSize().x;
    radius = (radius < 0.5) ? 0.5 : radius;
    m_shape.setRadius(radius);
    m_shape.setOrigin(radius, radius);

    //cout << "r " << radius << endl;
}

void PingPongBall::setColor(int r, int g, int b, int a)
{
    m_shape.setFillColor(sf::Color(r, g, b, a));
}

void PingPongBall::setVelocity(float vx, float vy)
{
    m_vx = vx, m_vy = vy;
}

void PingPongBall::setMass(float mass)
{
    m_mass = mass;
}

void PingPongBall::printStatus() const
{
    cout << "id " << m_id << " x " << m_x << " y " << m_y << " vx " << m_vx << " vy " << m_vy << endl;
}

pair<sf::Vector2f, sf::Vector2f> PingPongBall::getPredictedPosition(float dt) const
{
    float vx = m_vx, vy = m_vy;
    //if ( m_x+dt*m_vx < m_r || m_x+dt*m_vx > 1.0f - m_r ) vx = -m_vx;
    //if ( m_y+dt*m_vy < m_r || m_y+dt*m_vy > 1.0f - m_r ) vy = -m_vy;
    return make_pair(sf::Vector2f(m_x+dt*vx, m_y+dt*vy), sf::Vector2f(vx, vy));
}

float PingPongBall::getTimeToHitVerticalWall() const
{
    if (m_vx > 0)      return (1.0 - m_x - m_r) / m_vx;
    else if (m_vx < 0) return (m_r - m_x) / m_vx;
    else               return numeric_limits<float>::infinity();
}

float PingPongBall::getTimeToHitHorizontalWall() const
{
    float dt;
    if (m_vy > 0)      dt = (1.0 - m_y - m_r) / m_vy;
    else if (m_vy < 0) dt = (m_r - m_y) / m_vy;
    else               dt = numeric_limits<float>::infinity();
    return dt;
//    return dt > 0 ? dt : 0.f;
}

void PingPongBall::bounceOffVerticalWall()
{
    m_vx = -m_vx;
    m_count++;
}

void PingPongBall::bounceOffHorizontalWall()
{
    m_vy = -m_vy;
    m_count++;
}

float PingPongBall::getTimeToHit(std::shared_ptr<PingPongBall> that) const
{
    float INF = numeric_limits<float>::infinity();
    if (this == that.get()) return INF;
    float dx  = that->m_x - this->m_x;
    float dy  = that->m_y - this->m_y;
    float dvx = that->m_vx - this->m_vx;
    float dvy = that->m_vy - this->m_vy;
    float dvdr = dx*dvx + dy*dvy;
    if (dvdr > 0) return INF;
    float dvdv = dvx*dvx + dvy*dvy;
    if (dvdv == 0) return INF;
    float drdr = dx*dx + dy*dy;
    float sigma = this->m_r + that->m_r;
    float d = (dvdr*dvdr) - dvdv * (drdr - sigma*sigma);
    // if (drdr < sigma*sigma) StdOut.println("overlapping particles");
    if (d < 0) return INF;
    float dt = -(dvdr + sqrt(d)) / dvdv;
    float x = that->m_x + dt * that->m_vx;
    float y = that->m_y + dt * that->m_vy;
    if (x < that->m_r || x > 1.0-that->m_r || y < that->m_r || y > 1.0 - that->m_r) return INF;
    x = this->m_x + dt * this->m_vx;
    y = this->m_y + dt * this->m_vy;
    if (x < this->m_r || x > 1.0-this->m_r || y < this->m_r || y > 1.0 - this->m_r) return INF;
    return dt;
}

void PingPongBall::bounceOff(std::shared_ptr<PingPongBall> that)
{
    float dx  = that->m_x - this->m_x;
    float dy  = that->m_y - this->m_y;
    float dvx = that->m_vx - this->m_vx;
    float dvy = that->m_vy - this->m_vy;
    float dvdr = dx*dvx + dy*dvy;          // dv dot dr
    double dist = this->m_r + that->m_r;   // distance between particle centers at collison

    // magnitude of normal force
    double magnitude = 2 * this->m_mass * that->m_mass * dvdr / ((this->m_mass + that->m_mass) * dist);

    // normal force, and in x and y directions
    double fx = magnitude * dx / dist;
    double fy = magnitude * dy / dist;

    // update velocities according to normal force
    this->m_vx += fx / this->m_mass;
    this->m_vy += fy / this->m_mass;
    that->m_vx -= fx / that->m_mass;
    that->m_vy -= fy / that->m_mass;

    // update collision counts
    this->m_count++;
    that->m_count++;
}
