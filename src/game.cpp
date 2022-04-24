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

void battle(Player &p1, Player &p2) {
    Creature *c1 = p1.getCreature();
    Creature *c2 = p2.getCreature();
    // bool player1Done = false;
    // bool player2Done = false;
    int whichPlayer = 1;

    bool fighting = true;
    while (fighting) {
        int p1MoveUsed = 0;
        int p2MoveUsed = 0;
        bool asking = true;
        if (whichPlayer == 1) {
            printCreatureInfo(c1);
            while (asking) {
                cout << "Which move?: "; 
                cin >> p1MoveUsed;
                if (0 < p1MoveUsed && p1MoveUsed <= c1->getNumberOfMoves()) {
                    p1MoveUsed--;
                    asking = false;
                } else cout << "Invalid move!" << endl;
            }
            whichPlayer = 2;
        }

        if (whichPlayer == 2) {
            asking = true;
            printCreatureInfo(c2);
            while (asking) {
                cout << "Which move?: "; 
                cin >> p2MoveUsed;
                if (0 < p2MoveUsed && p2MoveUsed <= c2->getNumberOfMoves()) {
                    p2MoveUsed--;
                    asking = false;
                } else cout << "Invalid move!" << endl;
            }
            whichPlayer = 1;
        }

        cout << c1->getName() << " used " << c1->getMoveName(p1MoveUsed) << "!" << endl;
        cout << c2->getName() << " used " << c2->getMoveName(p2MoveUsed) << "!" << endl;

        dealDamage(c1, c2, c1->getMove(p1MoveUsed), c2->getMove(p2MoveUsed));
        fighting = (c1->getHealth() > 0 && c2->getHealth());
    }
}

}