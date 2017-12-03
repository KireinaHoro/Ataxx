#ifndef IATAXXPLAYER_H
#define IATAXXPLAYER_H

#include "gameprocess.h"

class IAtaxxPlayer
{
public:
    virtual GameProcess getMove(const GameProcess &current) = 0;
    virtual ~IAtaxxPlayer() {}

};

#endif // IATAXXPLAYER_H
