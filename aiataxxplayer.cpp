#include "aiataxxplayer.h"
#include "gameai.h"
#include "botzonerunner.h"

#include <iostream>
#include <string>

GameProcess AIAtaxxPlayer::getMove(const GameProcess &current)
{
    //BotzoneRunner::debug(current, "in getMove, before parallel");
    GameProcess ret = GameAI::runParallelAI(current);
    //BotzoneRunner::debug(ret, "in getMove, after parallel");
    char *move = new char[5];
    ret.getMoveStr(move);
    //qDebug((QString("AI made this move: ") + move).toStdString().c_str());
    std::cout << std::string("AI made this move: ") + move << std::endl;
    //BotzoneRunner::debug(game, "in clickedGamePiece, after waiting for thread");
    delete move;
    return ret;
}
