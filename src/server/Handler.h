// 315383133 shimon cohen
// 302228275 Nadav Spitzer

#ifndef ADVANCEDPROGRAMMING1_HANDLER_H
#define ADVANCEDPROGRAMMING1_HANDLER_H

#include "CommandsManager.h"
#include "Game.h"
#include <vector>
#include <string.h>
#include <cstdlib>

#define FIRST 1
#define SECOND 2
#define FOUR 4
#define BUFFERSIZE 200

/*
 * Holds a command and its arguments after extraction
 */
typedef struct CommandAndArgs {
    string command;
    vector<string> args;
}CommandAndArgs;

/*
 * Holds info needed by the static functions
 */
typedef struct HandleArgs {
    int socket;
    Game* game;
    vector<Game*> *games;
    vector<pthread_t*> *threadVector;
    pthread_mutex_t *gamesLock;
    pthread_mutex_t *threadsLock;
}HandleArgs;

/*
 * Handler is in charge of handling everything in the server from idividual clients to full games,
 * Also in charge of some of the aervers closing functions.
 */
class Handler {
public:
    /*
	 * function name: run.
	 * input: the number of the player's socket.
	 * output: none.
     * operation: starts the running of the thread.
    */
    void run(int clientSocket);
    /*
	 * function name: closeThreads.
	 * input: none.
	 * output: none.
     * operation: Stops the server's activity.
    */
    void closeThreads();
    /*
	 * function name: handleClient.
	 * input: socket of the client.
	 * output: none.
     * operation: reading the command from the user, extract it from the buffer and execute.
    */
    static void *handleClient(void* socket);
    /*
	 * function name: handleGame.
	 * input: a game to handle.
	 * output: none.
     * operation: reading the command from the user, extract it from the buffer and execute.
    */
    static void *handleGame(void* gameToHandle);
private:
    // the games running or waiting in the server
    vector<Game*> games;
    // the threads of the games in the server
    vector<pthread_t*> threadVector;
    /*
	 * function name: printThreadVectorSize.
	 * input: none.
	 * output: none.
     * operation: prints the current sizes of the thread vector and games vector.
    */
    void printThreadAndGamesVectorSize();
    /*
	 * function name: extractCommandAndArgs.
	 * input: a buffer string with the command the server read.
	 * output: a struct with the command and arguments received from the player.
     * operation: reading the command from the user, extract it from the buffer and execute.
    */
    static CommandAndArgs extractCommandAndArgs(char* buffer);
    /*
	 * function name: deleteGame.
	 * input: a vector of games, and a specific game to delete..
	 * output: none.
     * operation: deleting a specific game from the games vector.
    */
    static void deleteGame(vector<Game*> &games, string gameName);
    /*
	 * function name: deleteThread.
	 * input: a vector of threads, and a specific thread to delete.
	 * output: none.
     * operation: deleting a specific thread from the threads vector.
    */
    static void deleteThread(vector<pthread_t*> &threads, pthread_t pthread);
};


#endif //ADVANCEDPROGRAMMING1_HANDLER_H
