#include "game.h"
using namespace std;
Game::Game(string name)
    : m_window(sf::VideoMode(800, 800), name), m_name(name), m_nObject(0)
{
    //ctor
}

Game::~Game()
{
    //dtor
}

void Game::registerObject(std::shared_ptr<GameObject> pObject)
{
    int id = m_nObject;
    m_nObject++;

    m_objects.push_back(pObject);
    pObject->setID(id);
}

void Game::printStatus() const
{
    for (const auto& p : m_objects) {
        auto pObject = p.lock();
        if (pObject)
            pObject->printStatus();
    }
}

void Game::renderGame()
{
    m_window.clear();

    for (const auto& p : m_objects) {
        auto pObject = p.lock();
        if (pObject) {
//            pObject->printStatus();
            pObject->draw(m_window);
        }
    }

    m_window.display();
}
