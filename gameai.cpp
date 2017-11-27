#include "gameai.h"
#include <vector>

int GameAI::negamax(GameProcess current, int depth, int alpha, int beta, GameProcess *optimal)
{
    std::vector<GameProcess> moves;
    int tempscore;
    int maxscore = -50;
    GameProcess *best = nullptr;
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
        if (tempscore > maxscore)
        {
            maxscore = tempscore;
            alpha = std::max(alpha, tempscore);
            best = &i;
        }
        if (alpha >= beta)
            break;
    }

    *optimal = *best;

    return alpha;
}

GameProcess GameAI::runAI(GameProcess current)
{
    // the calling method shall check whether the game has finished; thus we do not check current.isFull() here

    // save the current state as the GameProcess objects created by negamax() will lose these information
    GameProcess::longint curr_lastboard = current.board;
    GameProcess::longint curr_lastactive = current.active;
    bool curr_player = current.player;

    negamax(current, NEGAMAX_DEPTH, -50, 50, &current);

    // update the state
    current.board = ~current.board;
    current.player = !curr_player;
    current.lastboard = curr_lastboard;
    current.lastactive = curr_lastactive;

    return current;
}
