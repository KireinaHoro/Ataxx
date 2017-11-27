#ifndef GAMEAI_H
#define GAMEAI_H

#include "gameprocess.h"

namespace GameAI {
int negamax(GameProcess current,
        int depth,
        int alpha,
        int beta,
        GameProcess* optimal);
const int NEGAMAX_DEPTH = 5;

GameProcess
runAI(GameProcess current);
}

#endif // GAMEAI_H
