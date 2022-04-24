#ifndef PLAYER_H_
#define PLAYER_H_

#include <string>
#include <array>

#include "creature.h"

namespace player {

class Player {
private:
    int uid;
    static int nextUID;
    char name[13];
    creature::Creature creature;
public:
    Player();
    Player(std::string _name, creature::Creature _creature);

    int getUID() const { return uid; }
    std::string getName() const { return std::string(name); }
    creature::Creature getCreature() { return creature; }

    void printInfo();
};

}

#endif