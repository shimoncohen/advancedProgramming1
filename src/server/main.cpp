// 315383133 shimon cohen
// 302228275 Nadav Spitzer

#include <string>
#include "Server.h"
#include <fstream>
#define ENDPORTWORD 5
using namespace std;

int settingsReading();

int main() {
    int port;
    port = settingsReading();
    Server server(port);
    try {
        // run the server
        server.runServer();
    } catch (const char *msg) {
        cout << "Cannot start server. Reason: " << msg << endl;
        exit(-1);
    }
    // stop the server from running
    server.stop();
    cout << "Server closed." << endl;
}

/*
    * function name: settingsReading.
    * input: none.
    * output: int the port number.
    * operation: reads the port number of the server from a file.
*/
int settingsReading() {
    //port number
    int port;
    //reader for the file
    string reader;
    //inpout streamer
    ifstream serverInfo;
    serverInfo.open("serverSettings.txt", ios::in);
    //error opening file will give a proper message
    if(!serverInfo.is_open()) {
        cout << "Error reading from file" << endl;
    //file opening was succssesful
    } else {
        //reading from the file
        while(!serverInfo.eof()) {
            serverInfo >> reader;
        }
        //removing "Port:" from the string
        reader = reader.erase(0, ENDPORTWORD);
    }
    //converting to int
    port = atoi(reader.c_str());
    serverInfo.close();
    return port;
}