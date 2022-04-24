#include <iostream>
#include <string>

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <netdb.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <getopt.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "creature.h"
#include "player.h"

using namespace std;

using namespace creature;
using namespace player;

const int MAXCLIENTS = 2;
// 1k of data
const int MAXBUFFERSIZE = 1025;

int main(int argc, char **argv) {
    int opt = true;
    int server_socket, activity, new_socket;
    int client_socket[MAXCLIENTS] = {0};
    int connected_clients = 0;
    int sd, max_sd;

    struct addrinfo hints, *res;
    struct sockaddr_in remote_address;
    char buffer[MAXBUFFERSIZE] = {0};

    fd_set readfds;

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    // Set server socket to allow multiple connections.
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt));

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    getaddrinfo(NULL, "4000", &hints, &res);    

    bind(server_socket, res->ai_addr, res->ai_addrlen);
    
    listen(server_socket, 2);

    Player p1;
    Player p2;
    
    unsigned int addrlen = sizeof(remote_address);
    while (connected_clients < 2) {
        FD_ZERO(&readfds);
        FD_SET(server_socket, &readfds);
        max_sd = server_socket;

        for (int i=0; i<MAXCLIENTS; i++) {
            sd = client_socket[i];

            if (sd > 0) FD_SET(sd, &readfds);
            if (sd > max_sd) max_sd = sd;
        }

        // Waits for activity on one of the sockets indefinitely
        activity = select(max_sd + 1, &readfds , NULL, NULL, NULL);

        if (activity < 0 && errno != EINTR) {  
            cerr << "select error" << endl;  
        }  

        if (FD_ISSET(server_socket, &readfds)) {
            new_socket = accept(server_socket, (struct sockaddr *)&remote_address, &addrlen);
            cout << "New connection added:" << endl;
            cout << "\tsocket: " << new_socket << endl;
            cout << "\tip: " << inet_ntoa(remote_address.sin_addr) << endl;
            cout << "\tport: " << ntohs(remote_address.sin_port) << endl; 
            client_socket[connected_clients] = new_socket;
            connected_clients++;
        }

    }

    read(client_socket[0], &p1, sizeof(Player));
    cout << "Recieved info from player 1" << endl;

    p1.printInfo();

    cout << "Accepted connection for player 2" << endl;
    read(client_socket[1], &p2, sizeof(Player));
    cout << "Recieved info from player 2" << endl;

    p2.printInfo();

    // strncpy(buffer, p2.getName().c_str(), MAXBUFFERSIZE);
    send(client_socket[0], &p2, sizeof(Player), 0);
    send(client_socket[1], &p1, sizeof(Player), 0);

    Creature c1 = p1.getCreature();
    Creature c2 = p2.getCreature();
    
    bool player1Turn = true;
    bool player2Turn = false;
    bool turnDone = false;

    bool fighting = true;
    while (fighting) {
        int p1MoveUsed = 0;
        int p2MoveUsed = 0;

        cout << "Sending clientTurn = " << player1Turn << " to player 1..." << endl;
        send(client_socket[0], &player1Turn, sizeof(player1Turn), 0);
        cout << "Player 1 recieved player1Turn." << endl;
        cout << "Sending clientTurn = " << player2Turn << " to player 2..." << endl;
        send(client_socket[1], &player2Turn, sizeof(player2Turn), 0);
        cout << "Player 2 recieved player2Turn.\n" << endl;
        if (player1Turn) {
            cout << "Sending turnDone = " << turnDone << " to player 2..." << endl;
            // Tells client 2 the turn is not done yet while they wait.
            send(client_socket[1], &turnDone, sizeof(turnDone), 0);
            cout << "Player 2 recieved turnDone." << endl;

            cout << "Player 1 is selecting a move..." << endl;
            read(client_socket[0], &p1MoveUsed, sizeof(p1MoveUsed));
            cout << "Player 1 has chosen their move." << endl;
            cout << "Sending turnDone = " << false << " to player 1..." << endl;
            // Telling client 1 the turn is not done after they selected a move.
            send(client_socket[0], &turnDone, sizeof(turnDone), 0);
            cout << "Player 1 recieved turnDone.\n" << endl;

            player1Turn = false;
            player2Turn = true;
        } else {
            turnDone = true;

            cout << "Sending turnDone = " << turnDone << " to player 1..." << endl;
            // Tells client 1 the turn will be done while they wait.
            send(client_socket[0], &turnDone, sizeof(turnDone), 0);
            cout << "Player 1 recieved turnDone." << endl;

            cout << "Player 2 is selecting a move..." << endl;
            read(client_socket[1], &p2MoveUsed, sizeof(p2MoveUsed));
            cout << "Player 2 has chosen their move." << endl;
            cout << "Sending turnDone = " << true << " to player 2..." << endl;
            // Telling client 2 the turn is done after they selected a move.
            send(client_socket[1], &turnDone, sizeof(turnDone), 0);
            cout << "Player 2 recieved turnDone.\n" << endl;

            player1Turn = true;
            player2Turn = false;
        }

        if (turnDone) {
            dealDamage(c1, c2, c1.getMove(p1MoveUsed), c2.getMove(p2MoveUsed));
            c1.printInfo();
            c2.printInfo();

            fighting = (c1.getHealth() > 0 && c2.getHealth() > 0);

            string tmp = c1.getName() + " used " + c1.getMoveName(p1MoveUsed) + "!\n";
            tmp += c2.getName() + " used " + c2.getMoveName(p2MoveUsed) + "!\n";
            if (!fighting) {
                bool player1Won = c1.getHealth() > 0;
                if (player1Won) tmp += p1.getName();
                else tmp += p2.getName();
                tmp += " won!\n";
            }
            strncpy(buffer, tmp.c_str(), MAXBUFFERSIZE);

            send(client_socket[0], &c1, sizeof(c1), 0);
            send(client_socket[0], &c2, sizeof(c2), 0);
            send(client_socket[0], &fighting, sizeof(fighting), 0);
            send(client_socket[0], &buffer, MAXBUFFERSIZE, 0);

            send(client_socket[1], &c1, sizeof(c1), 0);
            send(client_socket[1], &c2, sizeof(c2), 0);
            send(client_socket[1], &fighting, sizeof(fighting), 0);
            send(client_socket[1], &buffer, MAXBUFFERSIZE, 0);
        }
    }

    /* while (1) {
        unsigned int addrlen = sizeof(struct sockaddr_storage);
        client_socket = accept(server_socket, (struct sockaddr*)&remote_address, &addrlen);
        printf("Accepted connection\n");

        bool gameStarted = false;
        // Game logic
        if (gameStarted) {
            strncpy(buffer, "Hello!", strlen(buffer));
            send(client_socket, buffer, strlen(buffer), 0);
        }

        close(client_socket);
    } */

    cout << "Exiting server" << endl;
    return 0;
}