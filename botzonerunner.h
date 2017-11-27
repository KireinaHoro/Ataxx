#ifndef BOTZONERUNNER_H
#define BOTZONERUNNER_H

#include "gameprocess.h"


class BotzoneRunner
{
public:
    BotzoneRunner();

    void run();

private:
    void debug(GameProcess &state, const char *str);
};

#endif // BOTZONERUNNER_H
