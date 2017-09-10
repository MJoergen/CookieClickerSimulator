#include "CookieClicker.h"
#include "player.h"

#define TIME_STEP 10.0
#define TIME_MAX  1000

// To run this program, use e.g. the following command.
// CookieClicker 200000 40000000000
//
// The output below shows, that currently GoalCookiePlayer is
// the most efficient, reaching the goal at approximately
// one half of the time compared with GoalTimePlayer.
//
// Goal time:    200k
// Goal cookies: 40G
// 
// 
// 
// SimplePlayer
// Time    : 200k
// Cookies : 245M
// CPS     : 551k
// Cursor               -> 126  (667M) or (6.67G per CPS)
// Grandma              -> 113  (723M) or (723M per CPS)
// Farm                 ->  96  (739M) or (92.3M per CPS)
// Mine                 ->  79  (749M) or (15.9M per CPS)
// Factory              ->  62  (754M) or (2.9M per CPS)
// Bank                 ->  45  (754M) or (539k per CPS)
// Temple               ->  26  (757M) or (97.1k per CPS)
// Wizard Tower         ->   6  (763M) or (17.3k per CPS)
// Shipment             ->   0  (5.1G) or (19.6k per CPS)
// Alchemy Lab          ->   0  (75G) or (46.9k per CPS)
// Portal               ->   0  (1e+03G) or (100k per CPS)
// Time Machine         ->   0  (1.4e+04G) or (215k per CPS)
// Antimatter Condenser ->   0  (1.7e+05G) or (395k per CPS)
// 
// 
// 
// SmartPlayer
// Time    : 200k
// Cookies : 858M
// CPS     : 604k
// Cursor               -> 113  (108M) or (1.08G per CPS)
// Grandma              -> 115  (956M) or (956M per CPS)
// Farm                 ->  98  (977M) or (122M per CPS)
// Mine                 ->  80  (861M) or (18.3M per CPS)
// Factory              ->  63  (867M) or (3.33M per CPS)
// Bank                 ->  46  (867M) or (620k per CPS)
// Temple               ->  27  (871M) or (112k per CPS)
// Wizard Tower         ->   7  (878M) or (20k per CPS)
// Shipment             ->   0  (5.1G) or (19.6k per CPS)
// Alchemy Lab          ->   0  (75G) or (46.9k per CPS)
// Portal               ->   0  (1e+03G) or (100k per CPS)
// Time Machine         ->   0  (1.4e+04G) or (215k per CPS)
// Antimatter Condenser ->   0  (1.7e+05G) or (395k per CPS)
// 
// 
// 
// GoalCookiePlayer<40G>
// Time    : 95.7k
// Cookies : 40G
// CPS     : 1.4M
// Cursor               ->  39  (3.49k) or (34.9k per CPS)
// Grandma              ->  40  (26.8k) or (26.8k per CPS)
// Farm                 ->  39  (256k) or (32k per CPS)
// Mine                 ->  34  (1.39M) or (29.6k per CPS)
// Factory              ->  29  (7.48M) or (28.8k per CPS)
// Bank                 ->  24  (40.1M) or (28.6k per CPS)
// Temple               ->  18  (248M) or (31.7k per CPS)
// Wizard Tower         ->  10  (1.34G) or (30.3k per CPS)
// Shipment             ->   3  (7.76G) or (29.8k per CPS)
// Alchemy Lab          ->   0  (75G) or (46.9k per CPS)
// Portal               ->   0  (1e+03G) or (100k per CPS)
// Time Machine         ->   0  (1.4e+04G) or (215k per CPS)
// Antimatter Condenser ->   0  (1.7e+05G) or (395k per CPS)
// 
// 
// 
// GoalTimePlayer<200k>
// Time    : 149k
// Cookies : 40G
// CPS     : 3.2M
// Cursor               ->  52  (21.5k) or (215k per CPS)
// Grandma              ->  55  (218k) or (218k per CPS)
// Farm                 ->  52  (1.58M) or (197k per CPS)
// Mine                 ->  47  (8.55M) or (182k per CPS)
// Factory              ->  42  (46.1M) or (177k per CPS)
// Bank                 ->  36  (214M) or (153k per CPS)
// Temple               ->  28  (1G) or (128k per CPS)
// Wizard Tower         ->  19  (4.7G) or (107k per CPS)
// Shipment             ->   8  (15.6G) or (60k per CPS)
// Alchemy Lab          ->   0  (75G) or (46.9k per CPS)
// Portal               ->   0  (1e+03G) or (100k per CPS)
// Time Machine         ->   0  (1.4e+04G) or (215k per CPS)
// Antimatter Condenser ->   0  (1.7e+05G) or (395k per CPS)


static void PlayGame(CookieClickerGame &game, Player *player, float timeEnd, float cookieEnd) 
{
    std::cout << std::endl;
    std::cout << std::endl;
    player->Print(std::cout) << std::endl;
    game.Reset();
    while (true) {
        int unit = player->Play();
        if (unit == -1) {
            game.Click(10.0);
        }
        else {
            game.Buy(static_cast<e_unit>(unit));
        }
        game.PassTime(TIME_STEP);
        if ((game.GetTime() >= timeEnd or game.GetNumCookies() >= cookieEnd))
            break;
    }

    game.Show();

    game.ShowLog();
}

static float my_strtod(const char *str)
{
    char *pEnd;
    double val = strtod(str, &pEnd);
    switch (*pEnd)
    {
        case 'k' : val *= 1.0e3; break;
        case 'M' : val *= 1.0e6; break;
        case 'G' : val *= 1.0e9; break;
    }
    return val;
} // end of my_strtod

int main(int argc, char *argv[])
{
    if (argc < 3) {
        std::cout << "usage: CookieClicker <time_goal> <cookie_goal>" << std::endl;
        return -1;
    }

    float time_goal = my_strtod(argv[1]);
    float cookie_goal = my_strtod(argv[2]);
    std::cout << "Goal time:    " << CFloat(time_goal) << std::endl;
    std::cout << "Goal cookies: " << CFloat(cookie_goal) << std::endl;
    std::cout << std::endl;

    CookieClickerGame game;

    std::vector<Player *> players;
    players.push_back(new SimplePlayer(game));
    players.push_back(new SmartPlayer(game));

    {
    GoalCookiePlayer *player = new GoalCookiePlayer(game);
    player->SetCookieGoal(cookie_goal);
    players.push_back(player);
    }

    {
    GoalTimePlayer *player = new GoalTimePlayer(game);
    player->SetTimeGoal(time_goal);
    players.push_back(player);
    }

    for (std::vector<Player *>::iterator it = players.begin();
            it != players.end(); ++it)
    {
        PlayGame(game, *it, time_goal, cookie_goal);
    }

} // end of main

