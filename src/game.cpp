#include <iostream>

#include "creature.h"
#include "player.h"
#include "game.h"

using namespace std;

using namespace creature;
using namespace player;

namespace game {

void printCreatureInfo(Creature *c) {
    array<CreatureMove* , 4> moves = *c->getMoves();
    cout << c->getName() << ": " << c->getHealth() << endl; 
    cout << "Move:" << endl;
    for (unsigned i=0; i<4; i++) {
        if (moves[i]->getUID() != -1) {
            cout << "\t" << i+1 << ": " << moves[i]->getName() << "\tUses: " << static_cast<int>(moves[i]->getUses()) << endl;
        }
    }
    cout << endl;
}

void game() {}

}