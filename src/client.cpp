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

void game(char *serverIP) {
    string playerName = "";
    cout << "What is your name?: ";
    cin >> playerName;

    bool asking = true;
    int creatureNum = 0;
    while (asking) {
        cout << "Which creature do you want?" << endl;
        cout << "1. Lizard" << endl;
        cout << "2. Turtle" << endl;
        cout << "[1-2]: ";
        cin >> creatureNum;

        if (creatureNum > 0 && creatureNum <= 2) {
            asking = false;
        } else {
            cout << "Invalid choice, please try again." << endl;
        }
    }
    
    // These are just hardcoded for now.
    // Will have a file storing all information later on.
    Creature playerCreature;
    if (creatureNum == 1) {
        playerCreature = Creature("Lizard", "", FIRE, 39, 52, 43, 65);
        
        CreatureMove scratch("Scratch", PHYSICAL, NORMAL, OPPONENT, 40, 35, 100);
        CreatureMove leer("Leer", STATUS, NORMAL, OPPONENT, DEFENSE, -1, 30, 100);
        masterList.addMove(scratch);
        masterList.addMove(leer);
        playerCreature.addMove(scratch);
        playerCreature.addMove(leer);
    } else {
        playerCreature = Creature("Turtle", "", WATER, 44, 48, 65, 43);

        CreatureMove tackle("Tackle", PHYSICAL, NORMAL, OPPONENT, 50, 35, 100);
        CreatureMove growl("Growl", STATUS, NORMAL, OPPONENT, ATTACK, -1, 40, 100);
        masterList.addMove(tackle);
        masterList.addMove(growl);
        playerCreature.addMove(tackle);
        playerCreature.addMove(growl);  
    }
    // End of hardcoded things.

    Player p1(playerName, playerCreature);
    Player p2;

    int sock;
    struct sockaddr_in serv_addr;
    char buffer[MAXBUFFERSIZE] = {0};

    sock = socket(AF_INET, SOCK_STREAM, 0);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(4000);
    serv_addr.sin_addr.s_addr = inet_addr(serverIP);
    
    cout << "Waiting for server..." << endl;

    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    recv(sock, buffer, MAXBUFFERSIZE, 0);
    cout << buffer << endl;

    send(sock, &p1, sizeof(Player), 0);
    read(sock, &p2, sizeof(Player));

    cout << "Facing " << p2.getName() << endl;
    
    Creature c1 = p1.getCreature();
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
            cout << "Waiting for turn..." << endl;
        }

        if (turnDone) {
            read(sock, &c1, sizeof(c1));
            read(sock, &c2, sizeof(c2)); 
            read(sock, &fighting, sizeof(fighting));
            read(sock, buffer, MAXBUFFERSIZE);
            cout << buffer << endl;
        }
    }
}

int main(int argc, char **argv) {
    if (argc < 2) {
        cerr << "Please pass an IP address as an argument." << endl;
        return -1;
    }

    bool gameRunning = true;
    while(gameRunning) {
        int playerChoice = 0;
        cout << "What would you like to do?" << endl;
        cout << "1. Play game" << endl;
        cout << "2. Exit" << endl;
        cin >> playerChoice;
        if (playerChoice == 1) {
            game(argv[1]);
        } else if (playerChoice == 2) {
            cout << "Thanks for playing!" << endl;
            gameRunning = false;
        } else {
            cout << "Invalid choice, please try again." << endl;
        }
    }

    cout << "It worked?" << endl;
}