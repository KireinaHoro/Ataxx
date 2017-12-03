#ifndef AIATAXXPLAYER_H
#define AIATAXXPLAYER_H

#include "iataxxplayer.h"

class AIAtaxxPlayer : public IAtaxxPlayer
{
public:
    GameProcess getMove(const GameProcess &current) override;
};

#endif // AIATAXXPLAYER_H
