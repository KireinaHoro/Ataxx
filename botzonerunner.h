#ifndef BOTZONERUNNER_H
#define BOTZONERUNNER_H

#include "gameprocess.h"


class BotzoneRunner
{
public:
    BotzoneRunner();

    void run();

    static void debug(const GameProcess &state, const char *str);
};

#endif // BOTZONERUNNER_H
