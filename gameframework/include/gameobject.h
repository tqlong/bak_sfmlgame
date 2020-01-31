#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "game.h"
#include <memory>
#include <SFML/Graphics.hpp>

class Game;

class GameObject
{
    public:
        GameObject(std::shared_ptr<Game> pGame);
        virtual ~GameObject();

        virtual void draw(sf::RenderWindow& window) const {}
        virtual void printStatus() const;

        void setID(int id) { m_id = id; }
        int getID() const { return m_id; }
        std::shared_ptr<Game> getGame() const { return m_pGame.lock(); }
    protected:
        std::weak_ptr<Game> m_pGame;
        int m_id;

    private:
};

#endif // GAMEOBJECT_H
