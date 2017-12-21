//
// Created by shimon on 12/20/17.
//

#include "Server.h"

Server::Server(int port): port(port), serverSocket(0) {
    cout << "GameServer" << endl;
}

void Server::runServer() {
    // Create a socket point
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serverAddress;
    struct sockaddr_in firstClientAddress;
    //struct sockaddr_in secondClientAddress;
    socklen_t firstClientAddressLen;
    //socklen_t secondClientAddressLen;
    int n;
    pthread_t thread;
    if (serverSocket == -1) {
        throw "Error opening socket";
    }
    // Assign a local address to the socket
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
    while (true) {
        cout << "Waiting for client connections..." << endl;
        // Accept a new client connection
        int clientSocket = accept(serverSocket, (struct sockaddr *) &firstClientAddress, &firstClientAddressLen);
        n = pthread_create(&thread, NULL, handleClient, (void*)clientSocket);
        if (n) {
            cout << "Error: unable to create thread" << endl;
            exit(-1);
        }
        int i = 0;
        Game* game;
        for(i; i < games.size(); i++) {
            if(games[i].getFirstPlayer() == clientSocket || games[i].getSecondPlayer() == clientSocket) {
                game = &games[i];
            }
        }
        n = pthread_create(&thread, NULL, handleGame, (void*)game);
        if (n) {
            cout << "Error: unable to create thread" << endl;
            exit(-1);
        }
        //assigning the second player.
//        playerNum = SECOND;
//        int secondClientSocket = accept(serverSocket, (struct sockaddr *) &secondClientAddress,
//                                        &secondClientAddressLen);
//        n = write(secondClientSocket, &playerNum, sizeof(playerNum));
//        if (n == -1) {
//            cout << "Error writing to socket" << endl;
//            return;
//        }
    }
}

void* Server::handleClient(void* socket) {
    char* buffer[BUFFERSIZE];
    string command, arguments[SECOND];
    int i = 0, args = 0;
    int n = read((int)((int*)socket), buffer, BUFFERSIZE*sizeof(char));
    if (n == -1) {
        cout << "Error writing to socket" << endl;
        return NULL;
    }
    if (n == 0) {
        cout << "Client disconnected" << endl;
        return NULL;
    }
    for(i; i < BUFFERSIZE; i++) {
        if(buffer[i] != '\0') {
            if(buffer[i] != " ") {
                command = command + buffer[i];
            } else {
                break;
            }
        } else {
            break;
        }
    }
    for(i; i < BUFFERSIZE; i++) {
        if(buffer[i] != '\0') {
            if(buffer[i] != " ") {
                arguments[args] = arguments[args] + buffer[i];
            } else {
                args += 1;
            }
        } else {
            break;
        }
    }
//    int playerNum = FIRST;
//    int n = write((int)socket, &playerNum, sizeof(playerNum));
//    if (n == -1) {
//        cout << "Error writing to socket" << endl;
//        return NULL;
//    }
//    cout << "First player connected" << endl;
//    if ((int)socket == -1)
//        throw "Error on accept";
}

void* Server::handleGame(void* gameToHandle) {
    int n, x = 0, y = 0, temp;
    int i = 0, tempX = 0, tempY = 0, flag = 0;
    string startMessage = STARTMESSAGE, command;
    char* buffer[BUFFERSIZE];
    Game *currentGame = (Game*)gameToHandle;
    n = write(currentGame->getFirstPlayer(), &startMessage, sizeof(startMessage));
    if (n == -1) {
        cout << "Error writing to socket" << endl;
        return NULL;
    }
    n = write(currentGame->getSecondPlayer(), &startMessage, sizeof(startMessage));
    if (n == -1) {
        cout << "Error writing to socket" << endl;
        return NULL;
    }
    if (currentGame->getSecondPlayer() == -1)
        throw "Error on accept";
    int currentClient = currentGame->getFirstPlayer();
    int waitingClient = currentGame->getSecondPlayer();
    //while(x != -2 || y != -2) {
    do {
        //read the info sent from the client.
        int n = read(currentClient, buffer, BUFFERSIZE*sizeof(char));
        if (n == -1) {
            return NULL;
        }
        if (n == 0) {
            cout << "Client disconnected" << endl;
            return NULL;
        }
        while(buffer[i] != '\0') {
            command = command + buffer[i];
            i++;
        }
//        for(i; i < BUFFERSIZE && buffer[i] != '\0'; i++) {
//            if(flag == 1 && buffer[i] == ' ') {
//                flag = 2;
//            }
//            if(flag != 2 && buffer[i] >= 48 && buffer[i] <= 58) {
//                tempX *= tempX;
//                tempX += buffer[i] - 48;
//                flag = 1;
//            }
//            if(flag == 2 && buffer[i] >= 48 && buffer[i] <= 58) {
//                tempY *= tempY;
//                tempY += buffer[i] - 48;
//                flag = 1;
//            }
//        }
//        x = tempX;
//        y = tempY;
//        if(x == -1 && y == -1) {
//            cout << "Player has no move" << endl;
//        } else if(x == -2 && y == -2) {
//            cout << "Got end of game info" << endl;
//        } else {
//            cout << "Got info: " << "move " << x + 1 << " " << y + 1 << endl;
//        }
        temp = currentClient;
        currentClient = waitingClient;
        waitingClient = temp;
    } while(strcmp(command.c_str(), "End") != 0);

    ///////////////send command to CM//////////////

    // end of game
    // Close communication with the client.
    close(currentGame->getFirstPlayer());
    close(currentGame->getSecondPlayer());
}

void Server::stop() {
    close(serverSocket);
}

CommandsManager Server::getCommandManager() {
    return cm;
}

vector<Game> Server::getGameVector() {
    return games;
}