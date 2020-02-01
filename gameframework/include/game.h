#ifndef GAME_H
#define GAME_H

#include "gameobject.h"

#include <memory>
#include <string>
#include <unordered_map>
#include <SFML/Graphics.hpp>

class GameObject;

class Game
{
    public:
        Game(std::string name);
        virtual ~Game();

        virtual void createGame(std::shared_ptr<Game>) {}
        virtual void loop() {}

        sf::RenderWindow& getWindow() { return m_window; }
        void printStatus() const;

        void registerObject(std::shared_ptr<GameObject> pObject);
    protected:
        sf::RenderWindow m_window;
        std::string m_name;
        std::vector<std::weak_ptr<GameObject>> m_objects;

        void renderGame();
    private:
        int m_nObject;
};

#endif // GAME_H
