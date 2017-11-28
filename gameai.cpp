#include "gameai.h"
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

int GameAI::negamax(GameProcess current, int depth, int alpha, int beta, GameProcess &optimal)
{
    // premature termination of negamax due to time limit exceeding; return dummy value
    {
        std::lock_guard<std::mutex> _(timerMutex);
        if (timeToQuit)
            return MAGIC_NUMBER;
    }

    std::vector<GameProcess> moves;
    int tempscore;
    int maxscore = -50;
    GameProcess best;
    GameProcess::longint clonemoves = 0;
    GameProcess::longint jumpmoves;

    GameProcess::longint cells = current.board & current.active;

    // leaf
    if (!depth)
        return current.getScore();

    // find all possible clone moves
    // note: we'll use a different approach of getting the right answer instead of the following:
    /*
    for (int i = 0; i < 49; i ++)
       if (cells & GameProcess::mask[i])                // piece present
           clonemoves |= GameProcess::clonemask[i];     // mark clonable locations
    */
    // time is important when searching, and we only want to deal with non-zero bits instead of
    // every number; the following works better
    // a & -a      takes out the lowest non-zero bit of a (tips: consider -a in two's complement notation)
    // a &= a - 1  removes the lowest non-zero bit from a (tips: consider a - 1 decomposition)
    // GameProcess::ilog2(a) finds the highest bit in a

    // calculate all possible clone moves
    for (GameProcess::longint tempboard = cells; tempboard; tempboard &= tempboard - 1)
        clonemoves |= GameProcess::clonemask[GameProcess::ilog2(tempboard & -tempboard)];
    clonemoves &= ~current.active;                      // only pick vacant locations

    for (; clonemoves; clonemoves &= clonemoves - 1)
    {
        GameProcess::longint move = clonemoves & -clonemoves;
        moves.push_back(GameProcess(
                             cells | move | GameProcess::clonemask[GameProcess::ilog2(move)],
                             current.active | move
                         ));
    }

    // find and calculate all possible jump moves
    for (GameProcess::longint tempboard = cells; tempboard; tempboard &= tempboard - 1)
    {
        jumpmoves = GameProcess::jumpmask[GameProcess::ilog2(tempboard & -tempboard)];
        jumpmoves &= ~current.active;
        for (; jumpmoves; jumpmoves &= jumpmoves - 1)
        {
            GameProcess::longint move = jumpmoves & -jumpmoves;
            moves.push_back(GameProcess(
                                 cells | move | GameProcess::clonemask[GameProcess::ilog2(move)],
                                 (current.active | move) & ~(tempboard & -tempboard)
                             ));                        // remove the piece at source location
        }
    }

    // if no moves possible, "pass"
    if (!moves.size())
    {
        moves.push_back(GameProcess(
                             current.board,
                             current.active
                         ));
    }

    // run search through all possible moves
    for (auto &i : moves)
    {
        tempscore = -negamax(GameProcess(~i.board, i.active), depth - 1, -beta, -alpha, optimal);
        if (-tempscore == MAGIC_NUMBER)             // return dummy value if we got a dummy value
            return MAGIC_NUMBER;
        if (tempscore > maxscore)
        {
            maxscore = tempscore;
            alpha = std::max(alpha, tempscore);
            best = std::move(i);                    // C++11 move constructor
        }
        if (alpha >= beta)
            break;
    }

    optimal = std::move(best);

    return alpha;
}

void GameAI::runSingleAI(GameProcess current, int depth)
{
    // increment the thread counter
    {
        std::lock_guard<std::mutex> _(timerMutex);
        threadCount ++;
    }
    // save the current state as the GameProcess objects created by negamax() will lose these information
    GameProcess::longint curr_lastboard = current.board;
    GameProcess::longint curr_lastactive = current.active;
    bool curr_player = current.player;

    int ret = negamax(current, depth, -50, 50, current);

    if (ret != MAGIC_NUMBER)     // if the calculation completed in time
    {
        // update the state
        current.board = ~current.board;
        current.player = !curr_player;
        current.lastboard = curr_lastboard;
        current.lastactive = curr_lastactive;

        std::lock_guard<std::mutex> _(dataMutex);
        ans.push(current);
    }

    std::lock_guard<std::mutex> _(timerMutex);
    threadCount --;
    cv.notify_all();
}


GameProcess GameAI::runParallelAI(GameProcess current)
{
    using Clock = std::chrono::steady_clock;

    // fire up the threads
    for (int d = NEGAMAX_MIN_DEPTH; d <= NEGAMAX_MAX_DEPTH; d ++)
        std::thread(runSingleAI, current, d).detach();

    auto t0 = Clock::now();
    auto t1 = t0 + TIMEOUT;
    std::unique_lock<std::mutex> lk(timerMutex);
    while (!timeToQuit && Clock::now() < t1)
        cv.wait_until(lk, t1);
    timeToQuit = true;          // time's up; notify the workers

    while (threadCount > 0)
        cv.wait(lk);

    // it's safe to access ans directly as no threads are running for now
    return ans.top();
}

GameProcess GameAI::runAI(GameProcess current)
{
    return runParallelAI(current);
}
