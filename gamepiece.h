#ifndef GAMEPIECE_H
#define GAMEPIECE_H


class GamePiece
{
public:
    GamePiece();
    GamePiece(int x, int y, bool player) : x(x), y(y), player(player) { present = true; }

    bool isPresent() { return present; }

private:
    int x, y;
    bool player;
    bool present;   // if the piece is present
};

#endif // GAMEPIECE_H
