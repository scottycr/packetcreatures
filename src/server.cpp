#include <iostream>

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
#include "game.h"

using namespace std;

using namespace creature;
using namespace player;
using namespace game;

int main(int argc, char **argv) {
    int server_socket;
    int client_socket;
    struct addrinfo hints, *res;
    struct sockaddr_storage remote_address;
    // char buffer[1024] = {0};
    // int recv_len = 0;

    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    getaddrinfo(NULL, "1234", &hints, &res);    

    bind(server_socket, res->ai_addr, res->ai_addrlen);
    
    listen(server_socket, 1);

    Player p1;
    Player p2;

    unsigned int addrlen = sizeof(struct sockaddr_storage);
    client_socket = accept(server_socket, (struct sockaddr*)&remote_address, &addrlen);
    cout << "Accepted connection for player 1" << endl;
    read(client_socket, &p1, sizeof(Player));
    close(client_socket);
    cout << "Recieved info from player 1" << endl;

    p1.printInfo();

    addrlen = sizeof(struct sockaddr_storage);
    client_socket = accept(server_socket, (struct sockaddr*)&remote_address, &addrlen);
    cout << "Accepted connection for player 2" << endl;
    read(client_socket, &p2, sizeof(Player));
    close(client_socket);
    cout << "Recieved info from player 2" << endl;

    p2.printInfo();

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