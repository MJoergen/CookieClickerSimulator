#include "player.h"

std::ostream& operator <<(std::ostream &os, Player *player)
{
    return player->Print(os);
}

