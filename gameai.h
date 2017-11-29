#ifndef GAMEAI_H
#define GAMEAI_H

#include "gameprocess.h"
#include <stack>
#include <mutex>
#include <chrono>
#include <condition_variable>

namespace GameAI {
int negamax(GameProcess current,
        int depth,
        int alpha,
        int beta,
        GameProcess &optimal);

const int NEGAMAX_MIN_DEPTH = 5;
const int NEGAMAX_MAX_DEPTH = 13;
const int MAGIC_NUMBER = 0x3F3F3F3F;    // used to inform the caller function that timeout has been reached in negamax recursion
const auto TIMEOUT = std::chrono::milliseconds(950);

// for parallel processing multiple depths of searching
void runSingleAI(GameProcess current, int depth);
void runSerialAI(GameProcess current);
GameProcess runTimerAI(GameProcess current);

static int threadCount = 0;
static std::stack<GameProcess> ans;     // pool of answers from different search depths; the latest one will be chosen
static std::mutex timerMutex;           // mutex to keep the timer consistent
static std::condition_variable cv;      // condition variable for communication between worker and caller
static bool timeToQuit;                 // if it's time to quit

GameProcess runAI(GameProcess current);
}

#endif // GAMEAI_H
