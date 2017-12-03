#include "ataxxplayerthread.h"
#include "botzonerunner.h"

AtaxxPlayerThread::AtaxxPlayerThread(GameProcess g, IAtaxxPlayer *p)
    : game(g)
{
    player = p;
}

void AtaxxPlayerThread::run()
{
    GameProcess ret = game;
    //BotzoneRunner::debug(ret, "in run, before getMove");
    ret = player->getMove(ret);
    //BotzoneRunner::debug(ret, "in run, after getMove");
    qDebug("AI finished calculating");
    emit finished(ret);
}
