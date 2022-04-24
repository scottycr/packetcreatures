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

const int MAXBUFFERSIZE = 1025;

int main(int argc, char **argv) {
    // These are just hardcoded for now.
    // Will have a file storing all information later on.
    Creature turtle("Turtle", "", WATER, 44, 48, 65, 43);

    CreatureMove tackle("Tackle", PHYSICAL, NORMAL, 50, 35, 100);
    masterList.addMove(tackle);
    turtle.addMove(tackle);

    Player dude("Client2", turtle);
    // End of hardcoded things.

    int sock;
    struct sockaddr_in serv_addr;
    char buffer[MAXBUFFERSIZE] = {0};

    sock = socket(AF_INET, SOCK_STREAM, 0);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(4000);
    serv_addr.sin_addr.s_addr = inet_addr("18.218.184.65");

    Player p2;
    
    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    send(sock, &dude, sizeof(Player), 0);
    read(sock, &p2, sizeof(Player));

    cout << "Facing " << p2.getName() << endl;
    
    Creature c1 = dude.getCreature();
    Creature c2 = p2.getCreature();

    bool clientTurn = false;
    bool turnDone = false;

    bool fighting = true;
    while (fighting) {
        read(sock, &clientTurn, sizeof(clientTurn));
        if (clientTurn == true) {
            int moveUsed = 0;
            bool asking = true;
            c1.printMoveScreen();
            while (asking) {
                cout << "Which move?: "; 
                cin >> moveUsed;
                if (0 < moveUsed && moveUsed <= c1.getNumberOfMoves()) {
                    moveUsed--;
                    asking = false;
                } else cout << "Invalid move!" << endl;
            }
            send(sock, &moveUsed, sizeof(moveUsed), 0);
            // Checks if the turn is done.
            read(sock, &turnDone, sizeof(turnDone));
            cout << "Done with turn" << endl;
        } else { 
            cout << p2.getName() << " is selecting a move!" << endl;
            read(sock, &turnDone, sizeof(turnDone));
        }

        if (turnDone) {
            read(sock, &c2, sizeof(c2));
            read(sock, &c1, sizeof(c1));
            read(sock, &fighting, sizeof(fighting));
            read(sock, buffer, MAXBUFFERSIZE);
            cout << buffer << endl;
        }
    }

    cout << "It worked?" << endl;
}