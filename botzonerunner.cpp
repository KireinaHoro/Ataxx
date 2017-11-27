#include "botzonerunner.h"
#include "gameai.h"
#include <iostream>

BotzoneRunner::BotzoneRunner()
{

}

void BotzoneRunner::run()
{
    GameProcess state;
    int turnID;
    std::cin >> turnID;
    int start_x, start_y, end_x, end_y;

    // restore state to current turn
    for (int i = 0; i < turnID - 1; i ++)
    {
        std::cin >> start_x >> start_y >> end_x >> end_y;
        // the opponent places a piece
        if (end_x >= 0)
            state.doMove(start_x, start_y, end_x, end_y);

        // we place a piece
        std::cin >> start_x >> start_y >> end_x >> end_y;
        if (end_x >= 0)
            state.doMove(start_x, start_y, end_x, end_y);
    }

    // get current turn input
    std::cin >> start_x >> start_y >> end_x >> end_y;
    if (end_x >= 0)
        state.doMove(start_x, start_y, end_x, end_y);

    //debug(state, "BEFORE");

    // pick our choice
    state = GameAI::runAI(state);
    //state.doMove(0, 6, 0, 5);

    // parse the move performed
    char *move = new char[5];
    state.getMoveStr(move);

    //debug(state, "AFTER");

    std::cout << move[0] - 'a' << ' '
            << move[1] - '1' << ' '
            << move[2] - 'a' << ' '
            << move[3] - '1' << std::endl;

}

void BotzoneRunner::debug(GameProcess &state, const char *str)
{
    std::cout << "===== " << str << " =====" << std::endl;
    for (int i = 0; i < 7; i ++)
    {
        for (int j = 0; j < 7; j ++)
            std::cout << state.getPiece(j, i) << ' ';
        std::cout << std::endl;
    }
    std::cout << state.player << ' ' << state.board << ' ' << state.active << ' ' << state.lastactive << std::endl;
}
