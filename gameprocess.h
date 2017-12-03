#ifndef GAMEPROCESS_H
#define GAMEPROCESS_H

#include <string>

class GameProcess
{

    // utilities for bitwise operations
public:

#ifdef __LP64__
    using longint = long;

private:
    const static longint STARTBOARD    = 0b1000000000000000000000000000000000000000000000001l;
    const static longint STARTACTIVE   = 0b1000001000000000000000000000000000000000001000001l;
    const static longint FULLBOARD     = 0b1111111111111111111111111111111111111111111111111l;

#else
    using longint = long long;

private:
    const static longint STARTBOARD    = 0b1000000000000000000000000000000000000000000000001ll;
    const static longint STARTACTIVE   = 0b1000001000000000000000000000000000000000001000001ll;
    const static longint FULLBOARD     = 0b1111111111111111111111111111111111111111111111111ll;


#endif // (define __LP64__)

    static int score(longint board);    // return number of bits set in board

public:
    const static longint mask[];
    const static longint clonemask[];
    const static longint jumpmask[];
    static int ilog2(longint n);        // compute logarithm with base 2

    const static unsigned int BOARD_SIZE = 7;

    enum Piece
    {
        X, O, Absent
    };

    // chessboard data
    longint board;          // pieces for the current player
    longint lastboard;      // previous pieces for the current player, used when calculating the move performed
    longint active;         // pieces currently placed on the board
    longint lastactive;     // previous pieces placed, used when calculating the move performed
    bool    player;         // current player, true for X, false for O

    GameProcess();                                    // initializer
    GameProcess(longint board, longint active);       // initializer with start value
    GameProcess(bool first);                          // initializer with starting player

    void  getMoveStr(char *output);             // get the last move performed, storing into *output
    Piece getPiece(int x, int y);               // get the piece at (x,y): 1 for X, -1 for O, 0 for no piece
    int   getScore();                           // get the current score for the current player
    bool  isMoveAvailable();                    // check if further moves can be taken
    void  doMove(const char *move);             // perform the specified move on the board
    void  doMove(int u, int v, int x, int y);   // does the same thing as above, with different parameters
    bool  isValidMove(int u, int v, int x, int y);  // check if a move is valid
    bool  isEmpty();                            // check if the board is empty
    bool  isFull();                             // check if the board is full

    using StoreData = std::pair<std::string, size_t>;
    StoreData storeProcess();                 // store the current game process into a std::string
    void      loadProcess(StoreData data);       // load the game process from a std::string
};

#endif // GAMEPROCESS_H
