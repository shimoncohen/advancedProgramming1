//
// Created by shimon on 12/3/17.
//

#include "Server.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <stdio.h>
#define FIRST 1
#define SECOND 2

using namespace std;
#define MAX_CONNECTED_CLIENTS 10
Server::Server(int port): port(port), serverSocket(0) {
    cout << "Server" << endl;
}

void Server::start() {
// Create a socket point
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        throw "Error opening socket";
    }
    // Assign a local address to the socket
    struct sockaddr_in serverAddress;
    bzero((void *) &serverAddress, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(port);
    if (bind(serverSocket, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) == -1) {
        throw "Error on binding";
    }
    // Start listening to incoming connections
    listen(serverSocket, MAX_CONNECTED_CLIENTS);
// Define the client socket's structures
    struct sockaddr_in firstClientAddress;
    struct sockaddr_in secondClientAddress;
    socklen_t firstClientAddressLen;
    socklen_t secondClientAddressLen;
    int playerNum = FIRST;
    string board;
    int temp;
    int *x, *y;
    while (true) {
        cout << "Waiting for client connections..." << endl;
// Accept a new client connection
        int firstClientSocket = accept(serverSocket, (struct sockaddr *) &firstClientAddress, &firstClientAddressLen);
        int n = write(firstClientSocket, &playerNum, sizeof(playerNum));
        if (n == -1) {
            cout << "Error writing to socket" << endl;
            return;
        }
        cout << "First player connected" << endl;
        if (firstClientSocket == -1)
            throw "Error on accept";
        playerNum = SECOND;
        int secondClientSocket = accept(serverSocket, (struct sockaddr *) &secondClientAddress,
                                        &secondClientAddressLen);
        n = write(secondClientSocket, &playerNum, sizeof(playerNum));
        if (n == -1) {
            cout << "Error writing to socket" << endl;
            return;
        }
        cout << "Second player connected" << endl;
        if (secondClientSocket == -1)
            throw "Error on accept";
        int currentClient = firstClientSocket;
        int waitingClient = secondClientSocket;
        while(true) {
            handleClient(currentClient, x, y);
            writeToClient(waitingClient, x, y);
// Close communication with the client
            temp = currentClient;
            currentClient = waitingClient;
            waitingClient = temp;
            if(*x == -2 || *y == -2) {
                break;
            }
            *x = 0, *y = 0;
        }
        close(firstClientSocket);
        close(secondClientSocket);
    }
}

// Handle requests from a specific client
void Server::handleClient(int clientSocket, int *x, int *y) {
    //int waitingClient = secondClientSocket;
    //while (true) {
// Read new exercise arguments
    int n = read(clientSocket, &x, sizeof(x));
    if (n == -1) {
        cout << "Error reading x" << endl;
        return;
    }
    if (n == 0) {
        cout << "Client disconnected" << endl;
        return;
    }
    n = read(clientSocket, &y, sizeof(y));
    if (n == -1) {
        cout << "Error reading y" << endl;
        return;
    }
    cout << "Got info: " << "board and move " << *x << " " << *y << endl;
//        temp = currentClient;
//        currentClient = waitingClient;
//        waitingClient = temp;
    //}
}

void Server::writeToClient(int clientSocket, int *x, int *y) {
    int n = write(clientSocket, &x, sizeof(x));
    if (n == -1) {
        cout << "Error reading x" << endl;
        return;
    }
    if (n == 0) {
        cout << "Client disconnected" << endl;
        return;
    }
    n = write(clientSocket, &y, sizeof(y));
    if (n == -1) {
        cout << "Error reading y" << endl;
        return;
    }
    cout << "Wrote info: " << "board and move " << *x << " " << *y << endl;
}

void Server::stop() {
    close(serverSocket);
}