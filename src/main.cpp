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
#include "game.h"

using namespace std;

using namespace creature;
using namespace player;
using namespace game;

int main(int argc, char **argv) {
    // These are just hardcoded for now.
    // Will have a file storing all information later on.
    Creature lizard("Lizard", "", FIRE, 39, 52, 43, 65);
    array<CreatureMove*, 4> lizardMoves;
    Creature turtle("Turtle", "", WATER, 44, 48, 65, 43);
    array<CreatureMove*, 4> turtleMoves;
    
    CreatureMove scratch("Scratch", PHYSICAL, NORMAL, 40, 35, 100);
    masterList.addMove(&scratch);
    lizardMoves[0] = &scratch;
    lizardMoves[1] = masterList.getMove(-1);
    lizardMoves[2] = masterList.getMove(-1);
    lizardMoves[3] = masterList.getMove(-1);
    lizard.setMoves(lizardMoves);

    CreatureMove tackle("Tackle", PHYSICAL, NORMAL, 50, 35, 100);
    masterList.addMove(&tackle);
    turtleMoves[0] = &tackle;
    turtleMoves[1] = masterList.getMove(-1);
    turtleMoves[2] = masterList.getMove(-1);
    turtleMoves[3] = masterList.getMove(-1);
    turtle.setMoves(turtleMoves);

    Player bro("Bro", &lizard);
    Player dude("Dude", &turtle);
    // End of hardcoded things.  

    int sock;
    struct sockaddr_in serv_addr;
    // char buffer[1024] = {0};

    sock = socket(AF_INET, SOCK_STREAM, 0);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(1234);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    send(sock, &bro, sizeof(Player), 0);
    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    send(sock, &dude, sizeof(Player), 0);

    cout << "It worked?" << endl;
}