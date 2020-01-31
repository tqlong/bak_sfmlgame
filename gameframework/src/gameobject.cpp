#include "gameobject.h"

#include <iostream>
using namespace std;

GameObject::GameObject(std::shared_ptr<Game> pGame)
    : m_pGame(pGame)
{
}

GameObject::~GameObject()
{
    //dtor
}

void GameObject::printStatus() const
{
    cout << "object id = " << m_id << endl;
}
