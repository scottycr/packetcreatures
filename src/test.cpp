#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <getopt.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>

#include "creature.h"
#include "player.h"

using namespace std;

using namespace creature;
using namespace player;

int main(int argc, char **argv) {
    // These are just hardcoded for now.
    // Will have a file storing all information later on.
    Creature lizard("Lizard", "", FIRE, 39, 52, 43, 65);
    Creature turtle("Turtle", "", WATER, 44, 48, 65, 43);
    
    CreatureMove scratch("Scratch", PHYSICAL, NORMAL, OPPONENT, 40, 35, 100);
    CreatureMove leer("Leer", STATUS, NORMAL, OPPONENT, DEFENSE, -1, 30, 100);
    masterList.addMove(scratch);
    masterList.addMove(leer);
    lizard.addMove(scratch);
    lizard.addMove(leer);

    CreatureMove tackle("Tackle", PHYSICAL, NORMAL, OPPONENT, 50, 35, 100);
    CreatureMove growl("Growl", STATUS, NORMAL, OPPONENT, ATTACK, -1, 40, 100);
    masterList.addMove(tackle);
    masterList.addMove(growl);
    turtle.addMove(tackle);
    turtle.addMove(growl);

    Player p1("Player 1", lizard);
    Player p2("Turtle Man", turtle);
    // End of hardcoded things.  

    Creature c1 = p1.getCreature();
    Creature c2 = p2.getCreature();

    performTurn(c1, c2, c1.getMove(1), c2.getMove(0));
    performTurn(c1, c2, c1.getMove(1), c2.getMove(0));
    while (c1.getHealth() > 0 && c2.getHealth() > 0) {
        performTurn(c1, c2, c1.getMove(0), c2.getMove(0));
        cout << c1.getName() << ": " << c1.getHealth() << endl;
        cout << c2.getName() << ": " << c2.getHealth() << '\n' << endl;
    }
    // c1.printInfo();
    // c2.printInfo();
}