#ifndef GAME_H_
#define GAME_H_

#include "creature.h"
#include "player.h"

namespace game {

void printCreatureInfo(creature::Creature *c);

void battle(player::Player &p1, player::Player &p2);
    
}

#endif