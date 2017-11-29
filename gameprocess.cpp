#include "gameprocess.h"
#include <cstring>
#include <cstdlib>

#ifdef __LP64__

const GameProcess::longint GameProcess::mask[] =
{
    0x00000000000001l,
    0x00000000000002l,
    0x00000000000004l,
    0x00000000000008l,
    0x00000000000010l,
    0x00000000000020l,
    0x00000000000040l,
    0x00000000000080l,
    0x00000000000100l,
    0x00000000000200l,
    0x00000000000400l,
    0x00000000000800l,
    0x00000000001000l,
    0x00000000002000l,
    0x00000000004000l,
    0x00000000008000l,
    0x00000000010000l,
    0x00000000020000l,
    0x00000000040000l,
    0x00000000080000l,
    0x00000000100000l,
    0x00000000200000l,
    0x00000000400000l,
    0x00000000800000l,
    0x00000001000000l,
    0x00000002000000l,
    0x00000004000000l,
    0x00000008000000l,
    0x00000010000000l,
    0x00000020000000l,
    0x00000040000000l,
    0x00000080000000l,
    0x00000100000000l,
    0x00000200000000l,
    0x00000400000000l,
    0x00000800000000l,
    0x00001000000000l,
    0x00002000000000l,
    0x00004000000000l,
    0x00008000000000l,
    0x00010000000000l,
    0x00020000000000l,
    0x00040000000000l,
    0x00080000000000l,
    0x00100000000000l,
    0x00200000000000l,
    0x00400000000000l,
    0x00800000000000l,
    0x01000000000000l
};

const GameProcess::longint GameProcess::clonemask[] =
{
    0x00000000000182l,
    0x00000000000385l,
    0x0000000000070Al,
    0x00000000000E14l,
    0x00000000001C28l,
    0x00000000003850l,
    0x00000000003020l,
    0x0000000000C103l,
    0x0000000001C287l,
    0x0000000003850El,
    0x00000000070A1Cl,
    0x000000000E1438l,
    0x000000001C2870l,
    0x00000000181060l,
    0x00000000608180l,
    0x00000000E14380l,
    0x00000001C28700l,
    0x00000003850E00l,
    0x000000070A1C00l,
    0x0000000E143800l,
    0x0000000C083000l,
    0x0000003040C000l,
    0x00000070A1C000l,
    0x000000E1438000l,
    0x000001C2870000l,
    0x000003850E0000l,
    0x0000070A1C0000l,
    0x00000604180000l,
    0x00001820600000l,
    0x00003850E00000l,
    0x000070A1C00000l,
    0x0000E143800000l,
    0x0001C287000000l,
    0x0003850E000000l,
    0x0003020C000000l,
    0x000C1030000000l,
    0x001C2870000000l,
    0x003850E0000000l,
    0x0070A1C0000000l,
    0x00E14380000000l,
    0x01C28700000000l,
    0x01810600000000l,
    0x00081800000000l,
    0x00143800000000l,
    0x00287000000000l,
    0x0050E000000000l,
    0x00A1C000000000l,
    0x01438000000000l,
    0x00830000000000l
};

const GameProcess::longint GameProcess::jumpmask[] =
{
    0x0000000001C204l,
    0x0000000003C408l,
    0x0000000007C891l,
    0x000000000F9122l,
    0x000000001F2244l,
    0x000000001E0408l,
    0x000000001C0810l,
    0x00000000E10204l,
    0x00000001E20408l,
    0x00000003E44891l,
    0x00000007C89122l,
    0x0000000F912244l,
    0x0000000F020408l,
    0x0000000E040810l,
    0x00000070810207l,
    0x000000F102040Fl,
    0x000001F224489Fl,
    0x000003E448913El,
    0x000007C891227Cl,
    0x00000781020478l,
    0x00000702040870l,
    0x00003840810380l,
    0x00007881020780l,
    0x0000F912244F80l,
    0x0001F224489F00l,
    0x0003E448913E00l,
    0x0003C081023C00l,
    0x00038102043800l,
    0x001C204081C000l,
    0x003C408103C000l,
    0x007C891227C000l,
    0x00F912244F8000l,
    0x01F224489F0000l,
    0x01E040811E0000l,
    0x01C081021C0000l,
    0x00102040E00000l,
    0x00204081E00000l,
    0x00448913E00000l,
    0x00891227C00000l,
    0x0112244F800000l,
    0x0020408F000000l,
    0x0040810E000000l,
    0x00102070000000l,
    0x002040F0000000l,
    0x004489F0000000l,
    0x008913E0000000l,
    0x011227C0000000l,
    0x00204780000000l,
    0x00408700000000l
};

#else

const GameProcess::longint GameProcess::mask[] =
{
    0x00000000000001ll,
    0x00000000000002ll,
    0x00000000000004ll,
    0x00000000000008ll,
    0x00000000000010ll,
    0x00000000000020ll,
    0x00000000000040ll,
    0x00000000000080ll,
    0x00000000000100ll,
    0x00000000000200ll,
    0x00000000000400ll,
    0x00000000000800ll,
    0x00000000001000ll,
    0x00000000002000ll,
    0x00000000004000ll,
    0x00000000008000ll,
    0x00000000010000ll,
    0x00000000020000ll,
    0x00000000040000ll,
    0x00000000080000ll,
    0x00000000100000ll,
    0x00000000200000ll,
    0x00000000400000ll,
    0x00000000800000ll,
    0x00000001000000ll,
    0x00000002000000ll,
    0x00000004000000ll,
    0x00000008000000ll,
    0x00000010000000ll,
    0x00000020000000ll,
    0x00000040000000ll,
    0x00000080000000ll,
    0x00000100000000ll,
    0x00000200000000ll,
    0x00000400000000ll,
    0x00000800000000ll,
    0x00001000000000ll,
    0x00002000000000ll,
    0x00004000000000ll,
    0x00008000000000ll,
    0x00010000000000ll,
    0x00020000000000ll,
    0x00040000000000ll,
    0x00080000000000ll,
    0x00100000000000ll,
    0x00200000000000ll,
    0x00400000000000ll,
    0x00800000000000ll,
    0x01000000000000ll
};

const GameProcess::longint GameProcess::clonemask[] =
{
    0x00000000000182ll,
    0x00000000000385ll,
    0x0000000000070All,
    0x00000000000E14ll,
    0x00000000001C28ll,
    0x00000000003850ll,
    0x00000000003020ll,
    0x0000000000C103ll,
    0x0000000001C287ll,
    0x0000000003850Ell,
    0x00000000070A1Cll,
    0x000000000E1438ll,
    0x000000001C2870ll,
    0x00000000181060ll,
    0x00000000608180ll,
    0x00000000E14380ll,
    0x00000001C28700ll,
    0x00000003850E00ll,
    0x000000070A1C00ll,
    0x0000000E143800ll,
    0x0000000C083000ll,
    0x0000003040C000ll,
    0x00000070A1C000ll,
    0x000000E1438000ll,
    0x000001C2870000ll,
    0x000003850E0000ll,
    0x0000070A1C0000ll,
    0x00000604180000ll,
    0x00001820600000ll,
    0x00003850E00000ll,
    0x000070A1C00000ll,
    0x0000E143800000ll,
    0x0001C287000000ll,
    0x0003850E000000ll,
    0x0003020C000000ll,
    0x000C1030000000ll,
    0x001C2870000000ll,
    0x003850E0000000ll,
    0x0070A1C0000000ll,
    0x00E14380000000ll,
    0x01C28700000000ll,
    0x01810600000000ll,
    0x00081800000000ll,
    0x00143800000000ll,
    0x00287000000000ll,
    0x0050E000000000ll,
    0x00A1C000000000ll,
    0x01438000000000ll,
    0x00830000000000ll
};

const GameProcess::longint GameProcess::jumpmask[] =
{
    0x0000000001C204ll,
    0x0000000003C408ll,
    0x0000000007C891ll,
    0x000000000F9122ll,
    0x000000001F2244ll,
    0x000000001E0408ll,
    0x000000001C0810ll,
    0x00000000E10204ll,
    0x00000001E20408ll,
    0x00000003E44891ll,
    0x00000007C89122ll,
    0x0000000F912244ll,
    0x0000000F020408ll,
    0x0000000E040810ll,
    0x00000070810207ll,
    0x000000F102040Fll,
    0x000001F224489Fll,
    0x000003E448913Ell,
    0x000007C891227Cll,
    0x00000781020478ll,
    0x00000702040870ll,
    0x00003840810380ll,
    0x00007881020780ll,
    0x0000F912244F80ll,
    0x0001F224489F00ll,
    0x0003E448913E00ll,
    0x0003C081023C00ll,
    0x00038102043800ll,
    0x001C204081C000ll,
    0x003C408103C000ll,
    0x007C891227C000ll,
    0x00F912244F8000ll,
    0x01F224489F0000ll,
    0x01E040811E0000ll,
    0x01C081021C0000ll,
    0x00102040E00000ll,
    0x00204081E00000ll,
    0x00448913E00000ll,
    0x00891227C00000ll,
    0x0112244F800000ll,
    0x0020408F000000ll,
    0x0040810E000000ll,
    0x00102070000000ll,
    0x002040F0000000ll,
    0x004489F0000000ll,
    0x008913E0000000ll,
    0x011227C0000000ll,
    0x00204780000000ll,
    0x00408700000000ll
};

#endif // (define __LP64__)

int GameProcess::ilog2(longint n)
{
    int k = 0;
    int i = 32;
    while (i)
    {
        if (n >> i)
        {
            k += i;
            n >>= i;
        }
        i >>= 1;
    }
    return k;
}

int GameProcess::score(longint board)
{
    int score;
    for (score = 0; board; board &= board - 1)
        score ++;
    return score;
}


GameProcess::GameProcess()
{
    board       = STARTBOARD;
    lastboard   = STARTBOARD;
    active      = STARTACTIVE;
    lastactive  = STARTACTIVE;
    player      = true;
}

GameProcess::GameProcess(longint board, longint active) : board(board), active(active)
{
    lastboard   = STARTBOARD;
    lastactive  = STARTACTIVE;
    player      = true;
}

void GameProcess::getMoveStr(char *output)
{
    if ((active ^ lastactive) & (~active))     // jump move
    {
        for (int i = 0; i < 7; i ++)
            for (int j = 0; j < 7; j ++)
            {
                if ((~active) & lastactive & mask[7 * i + j]) // source
                {
                    output[0] = char('a' + i);
                    output[1] = char('1' + j);
                }
                if (active & (~lastactive) & mask[7 * i + j]) // destination
                {
                    output[2] = char('a' + i);
                    output[3] = char('1' + j);
                }
            }
    }
    else if ((active ^ lastactive) & (active)) // clone move
    {
        for (int i = 0; i < 7; i ++)
            for (int j = 0; j < 7; j ++)
                if (active & (~lastactive) & mask[7 * i + j]) // destination
                {
                    for (int k = 0; k < 7; k ++)              // find source: blame Ataxx game on Botzone!
                        for (int l = 0; l < 7; l ++)
                        {
                            // we need to use lastboard here as board can be affected by current move
                            if (((active & lastboard) & clonemask[7 * i + j]) & mask[7 * k + l])
                            {
                                output[0] = char('a' + k);
                                output[1] = char('1' + l);
                            }
                        }
                    output[2] = char('a' + i);
                    output[3] = char('1' + j);
                }
    }
    else                                       // passed move
        strcpy(output, "pass");
}

int GameProcess::getPiece(int x, int y)
{
    if (active & mask[7 * x + y])       // piece present
    {
        if(board & mask[7 * x + y])     // piece for X
            return player ? 1 : -1;
        else                            // piece for O
            return player ? -1 : 1;
    }
    else                                // piece not present
        return 0;
}

int GameProcess::getScore()
{
    return score(board & active) - score(~board & active);
}

int GameProcess::getAvailableMoves()
{
    int nummoves = 0;
    longint clonemoves = 0;         // all location valid for clone moves
    longint jumpmoves;              // location valid for jump moves for each piece
    longint cells = board & active; // pieces that belong to the current player

    // for clone moves, multiple locations that clone to the same location are considered same moves,
    // thus the count needs to be calculated after all the posssible location resolution
    for (int i = 0; i < 49; i ++)
        if (cells & mask[i])        // piece present
            clonemoves |= clonemask[i];
    clonemoves &= ~active;          // only pick vacant locations
    for (int i = 0; i < 49; i ++)
        if (clonemoves & mask[i])
            nummoves ++;

    // for jump moves, different source locations mean different moves, even if they jump to the same
    // destination, thus the count should be calculated for each source location
    for (int i = 0; i < 49; i ++)
        if (cells & mask[i])
        {
            jumpmoves = jumpmask[i] & ~active;
            for (int j = 0; j < 49; j ++)
                if (jumpmoves & mask[i])
                    nummoves ++;
        }

    return nummoves;
}

void GameProcess::doMove(const char *move)
{
    lastboard = board;               // last state should be updated even if move is "pass"
    lastactive = active;             // last state should be updated even if move is "pass"
    if (strcmp(move, "pass"))
    {
        int x, y, u, v;
        y = move[3] - '1';
        x = move[2] - 'a';
        v = move[1] - '1';
        u = move[0] - 'a';
        board |= mask[7 * x + y] | clonemask[7 * x + y];      // place piece at (x, y) and infect pieces around
        active |= mask[7 * x + y];                            // mark (x, y) as occupied
        if (std::abs(x - u) > 1 || std::abs(y - v) > 1)  // a jump move
        {
            active &= ~mask[7 * u + v];                       // remove piece at (x, y)
        }
    }
    // next player's turn
    board = ~board;
    player = !player;
}

void GameProcess::doMove(int u, int v, int x, int y)
{
    char *move = (char*)calloc(5, sizeof(char));
    move[0] = char(u + 'a');
    move[1] = char(v + '1');
    move[2] = char(x + 'a');
    move[3] = char(y + '1');
    doMove(move);
    free(move);
}

bool GameProcess::isFull()
{
    return active == FULLBOARD;
}
