// 315383133 shimon cohen
// 302228275 Nadav Spitzer

#include "Board.h"

Board::Board(int size) {
    boardSize = size;
    board = new boardChar*[size];
    for(int i = 0; i < size; i++) {
        board[i] = new boardChar[size];
    }
    //fill the board cells with spaces to indicate free spaces.
    for(int i = 0; i < size; i++) {
        for(int k = 0; k < size; k++) {
            board[i][k] = space;
        }
    }
    board[size / 2 - 1][size / 2 - 1] = white;
    board[size / 2 - 1][size / 2] = black;
    board[size / 2][size / 2 - 1] = black;
    board[size / 2][size / 2] = white;
}

Board::Board() {
    boardSize = DEFAULTSIZE;
    board = new boardChar*[DEFAULTSIZE];
    for(int i = 0; i < DEFAULTSIZE; i++) {
        board[i] = new boardChar[DEFAULTSIZE];
    }
    //fill the board cells with spaces to indicate free spaces.
    for(int i = 0; i < DEFAULTSIZE; i++) {
        for(int k = 0; k < DEFAULTSIZE; k++) {
            board[i][k] = space;
        }
    }
    board[DEFAULTSIZE / 2 - 1][DEFAULTSIZE / 2 - 1] = white;
    board[DEFAULTSIZE / 2 - 1][DEFAULTSIZE / 2] = black;
    board[DEFAULTSIZE / 2][DEFAULTSIZE / 2 - 1] = black;
    board[DEFAULTSIZE / 2][DEFAULTSIZE / 2] = white;
}

Board::Board(Board &board1) {
    boardSize = board1.getSize();
    board = new boardChar *[boardSize];
    // initialize cells.
    for (int i = 0; i < boardSize; i++) {
        board[i] = new boardChar[boardSize];
    }
    // copy the given boards cell values to current board.
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            if (board1.checkCell(i, j) == 'x') {
                board[i][j] = black;
            } else if (board1.checkCell(i, j) == 'o') {
                board[i][j] = white;
            } else {
                board[i][j] = space;
            }
        }
    }
}

Board::~Board() {
    for(int i = 0; i < boardSize; i++) {
        delete[] board[i];
    }
    delete[] board;
}

int Board::getSize() {
    return boardSize;
}

char Board::checkCell(const int x, const int y) {
    //if the cell is out of the boards bounds.
    if (x < 0 || x >= boardSize || y < 0 || y >= boardSize) {
        return ' ';
    }
    // return the cells value.
    if (board[x][y] == black) {
        return 'x';
    } else if (board[x][y] == white) {
        return 'o';
    }
    return ' ';
}

void Board::putTile(const int x, const int y, const type type) {
    // put a value in the cell according to type.
    if(type == blackPlayer) {
        board[x][y] = black;
    } else if(type == whitePlayer) {
        board[x][y] = white;
    } else {
        board[x][y] == space;
    }
}

ostream &operator <<(ostream &out, Board &board1) {
    cout << "Current board:" << endl << endl;
    // print the board to the stream with all
    // of the line and column numbers and dividers between cells.
    if(board1.getSize() > 9) {
        out << "  ";
    } else {
        out << " ";
    }
    for(int i = 1; i <= board1.getSize(); i++) {
        out << "| " << i << " ";
    }
    out << "|" << endl;
    out << "--";
    for(int k = 0; k < board1.getSize(); k++) {
        if(k > 8) {
            out << "-----";
            continue;
        }
        out << "----";
    }
    out << endl;
    for(int i = 0; i < board1.getSize(); i++) {
        out << i + 1;
        if(board1.getSize() > 9 && i < 9) {
            cout << " ";
        }
        out << "| ";
        for(int k = 0; k < board1.getSize(); k++) {
            out << board1.checkCell(i, k);
            if(k > 7) {
                out << " |  ";
            } else {
                out << " | ";
            }
            if(k == board1.getSize() - 1) {
                out << endl;
            }
        }
        out << "--";
        for(int k = 0; k < board1.getSize(); k++) {
            if(k > 8) {
                out << "-----";
                continue;
            }
            out << "----";
        }
        out << endl;
    }
    return out;
}

bool operator ==(Board &board1, Board &board2) {
    int i = 0, k = 0;
    // go over the board and compare each cell.
    for(i; i < board1.getSize(); i++) {
        for(k; k < board1.getSize(); k++) {
            // if even one cell is diffrent return false.
            if(board1.checkCell(i,k) != board2.checkCell(i, k)) {
                return false;
            }
        }
    }
    // if all cells in both boards are the same return true.
    return true;
}