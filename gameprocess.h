#ifndef GAMEPROCESS_H
#define GAMEPROCESS_H

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

    // chessboard data
    longint board;          // pieces for the current player
    longint lastboard;      // previous pieces for the current player, used when calculating the move performed
    longint active;         // pieces currently placed on the board
    longint lastactive;     // previous pieces placed, used when calculating the move performed
    bool    player;         // current player, true for X, false for O

    GameProcess();                             // initializer
    GameProcess(longint board, longint active);        // initializer with start value

    void getMoveStr(char *output);             // get the last move performed, storing into *output
    int  getPiece(int x, int y);               // get the piece at (x,y): 1 for X, -1 for O, 0 for no piece
    int  getScore();                           // get the current score for the current player
    int  getAvailableMoves();                  // get the number of valid moves that can be taken
    void doMove(const char *move);             // perform the specified move on the board
    void doMove(int u, int v, int x, int y);   // does the same thing as above, with different parameters
    bool isFull();                             // judge if the board is full
};

#endif // GAMEPROCESS_H
