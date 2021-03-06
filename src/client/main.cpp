// 315383133 shimon cohen
// 302228275 Nadav Spitzer

#include "Game.h"
#include <fstream>
#define ENDPORTWORD 5
#define ENDIPWORD 3

using namespace std;

void settingsReading(int* port, char *IPAddress);

int main() {
    char *IPAddress = (char*)malloc(255 * sizeof(char));
	int choice = -1, port;
    //int size;
    settingsReading(&port, IPAddress);
    Printer *printer = new ConsolePrinter();
    printer->printGameOpenning();
    // ***for future use when board size will be decided by the user***
    // getting board size from the user
    // printer->boardSizeMessage();
    // cin >> size;

    // creating game logic
    GameLogic* gameLogic = new StandartGameLogic();
    Player *first;
    Player *second;
    Game *g;
    // creating the second player according to the game style (Player/ AI).
    // create a game object and give it a board, players and a logic to play by.
    do {
        cin >> choice;
        switch (choice) {
            // human vs. human
            case 1:
                first = new HumanPlayer();
                second = new HumanPlayer();
                g = new Game(gameLogic, first, second);
                break;
                // human vs. AIPlayer
            case 2:
                first = new HumanPlayer();
                second = new AIPlayer();
                g = new Game(gameLogic, first, second);
                break;
                // Game on server.
                // Play against a remote player.
            case 3:
                first = new HumanPlayer();
                second = NULL;
                try {
                    second = new ServerPlayer(IPAddress, port);
                } catch (const char *msg) {
                    printer->failedMessage(msg);
                    if (strcmp(msg, "You requested to cancel.\n") == 0) {
                        choice = 0;
                        printer->printGameOpenning();
                    }
                }
                g = new Game(gameLogic, first, second);
                break;
            default:
                printer->printInvalidGameMode();
        }
    } while(choice != 1 && choice != 2 && choice != 3);
    // if there was no error starting the game.
    if(second != NULL) {
        try {
            g->runGame();
        } catch (const char *msg) {
            printer->failedMessage(msg);
        }
    }
    delete printer;
    delete g;
    free(IPAddress);
	return 0;
}

/*
    * function name: settingsReading.
    * input: int* port, const char** IPAddress.
    * output: none.
    * operation: reads the port number, and IP address of the server from a file.
*/
void settingsReading(int* port, char *IPAddress) {
    //reader for the file
    string reader;
    //input streamer
    ifstream serverInfo;
    serverInfo.open("serverInfo.txt", ios::in);
    //error opening file will give a proper message
    if(!serverInfo.is_open()) {
        cout << "Error reading from file" << endl;
        //file opening was succssesful
    } else {
        //reading from the file
        while(!serverInfo.eof()) {
            serverInfo >> reader;
            if(reader.find("IP:", 0) == 0) {
                //removing "IP:" from the string
                reader = reader.erase(0, ENDIPWORD);
                strcpy(IPAddress, reader.c_str());
            } else if(reader.find("Port:", 0) == 0) {
                //removing "Port:" from the string
                reader = reader.erase(0, ENDPORTWORD);
                //converting to int
                *port = atoi(reader.c_str());
            }
        }
    }
    serverInfo.close();
}