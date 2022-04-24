#include <string>
#include <cstring>
#include <iostream>

#include "creature.h"
#include "player.h"

using namespace std;
using namespace creature;
using namespace player;

int Player::nextUID = 0;

Player::Player() :
    uid(-1),
    name("empty"),
    creature(Creature())
{
    name[MAXNAMESIZE-1] = '\0';
}

Player::Player(string _name, Creature _creature) :
    uid(nextUID),
    creature(_creature)
{
    nextUID++;
    strncpy(name, _name.c_str(), MAXNAMESIZE);
    name[MAXNAMESIZE-1] = '\0';
}

void Player::printInfo() {
    cout << uid << ' ' << name << endl;
    cout << endl;
    creature.printInfo();
}
