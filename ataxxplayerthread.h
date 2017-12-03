#ifndef ATAXXPLAYERTHREAD_H
#define ATAXXPLAYERTHREAD_H

#include <QThread>

#include <memory>

#include "gameprocess.h"
#include "iataxxplayer.h"

class AtaxxPlayerThread : public QThread
{
    Q_OBJECT

public:
    AtaxxPlayerThread(GameProcess g, IAtaxxPlayer *p);
    void run();

signals:
    void finished(GameProcess);

private:
    IAtaxxPlayer *player = nullptr;
    GameProcess game;
};

#endif // ATAXXPLAYERTHREAD_H
