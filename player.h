#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <time.h>
#include <stdlib.h>
#include <assert.h>

#include "CookieClicker.h"

class Player
{
protected:
    CookieClickerGame &m_game;

public:
    Player(CookieClickerGame &game) : m_game(game) {}

    virtual std::ostream& Print(std::ostream &os) const = 0;
    virtual int Play() const = 0;
};

class SimplePlayer : public Player
{
    // Simple strategy: Always try to buy the most expensive unit.
    // Only buy one at each time step, because otherwise we'll never be
    // able to afford the expensive ones, when we keep spending our cookies
    // on the cheap ones.
    // TEST: This strategy achieves 410.3 M cookies at time 200 k.
public:
    SimplePlayer(CookieClickerGame &game) : Player(game) {}

    std::ostream& Print(std::ostream &os) const {
        return os << "SimplePlayer";
    }

    int Play() const
    {
        int unit = UNIT_ANTIMATTER_CONDENSER;
        while (true)
        {
            if (m_game.GetNumCookies() >= m_game.GetUnitPrice(static_cast<e_unit>(unit)))
            {
                return unit;
            }
            if (unit == UNIT_CURSOR)
                break;
            unit--;
        }

        // Couldn't afford anything. Press ten times on mouse instead.
        return -1;
    }
}; // end of SimplePlayer

class SmartPlayer : public Player
{
    // Smart (?) strategy: 
    // Among the units we can afford, choose the one with the lowest cost per CPS.
    // TEST: This strategy achieves 3940.3 M cookies at time 200 k.
public:
    std::ostream& Print(std::ostream &os) const {
        return os << "SmartPlayer";
    }

    SmartPlayer(CookieClickerGame &game) : Player(game) {}

    int Play() const
    {
        int best_unit = -1;
        float best_cost_per_cps = 999999999;
        for (int unit = UNIT_CURSOR; unit <= UNIT_ANTIMATTER_CONDENSER; ++unit)
        {
            float cost = m_game.GetUnitPrice(static_cast<e_unit>(unit));
            float cps = m_game.GetUnitCPS(static_cast<e_unit>(unit));
            float cost_per_cps = cost / cps;
            if (m_game.GetNumCookies() >= cost)
            {
                if (cost_per_cps < best_cost_per_cps)
                {
                    best_cost_per_cps = cost_per_cps;
                    best_unit = unit;
                }
            }
        }

        return best_unit;
    }
}; // end of SmartPlayer

class GoalCookiePlayer : public Player
{
    // Strategy: 
    // The goal is to reach the maximum number of cookies at a certain time.
    // TEST: This strategy achieves 40 G cookies at time 28.4 k.
private:
    float m_cookie_goal;

public:
    std::ostream& Print(std::ostream &os) const {
        return os << "GoalCookiePlayer<" << CFloat(m_cookie_goal) << ">";
    }

    GoalCookiePlayer(CookieClickerGame &game) : Player(game) {}

    void SetCookieGoal(float cookie_goal) {m_cookie_goal = cookie_goal;}

    // Determine the most optimal unit we would like to buy, to reach our goal.
    int GetGoal(float goal) const
    {
        float cookies_needed = goal - m_game.GetNumCookies();

        assert(cookies_needed > 0.0);

        int best_unit = -1;
        float best_time_needed = cookies_needed / m_game.GetCPS();
        for (int unit = UNIT_CURSOR; unit <= UNIT_ANTIMATTER_CONDENSER; ++unit)
        {
            float cost = m_game.GetUnitPrice(static_cast<e_unit>(unit));
            float cps = m_game.GetUnitCPS(static_cast<e_unit>(unit));

            if (goal > cost) {
                float time_needed = (cookies_needed + cost)/(m_game.GetCPS() + cps);
                if (time_needed < best_time_needed)
                {
                    best_time_needed = time_needed;
                    best_unit = unit;
                }
            }
        }

        return best_unit;
    }

    // If we can't afford the most optimal unit, then set a new goal that is the price of this unit.
    int Play() const
    {
        float goal = m_cookie_goal;

        while (true)
        {
            int unit = GetGoal(goal);
            if (unit == -1)
                return unit;

            float price = m_game.GetUnitPrice(static_cast<e_unit>(unit));
            if (m_game.GetNumCookies() >= price)
            {
                // If we can afford the unit, buy it!
                return unit;
            }

            goal = price;
        }
    }
}; // end of GoalCookiePlayer

class GoalTimePlayer : public Player
{
    // Strategy: 
    // The goal is to reach a certain number of cookies in the shortest possible time.
    // TEST: This strategy achieves 40 G cookies at time 79.3 k.
    // Therefore, this is currently inferior to GoalCookiePlayer.
private:
    float m_time_goal;

public:
    std::ostream& Print(std::ostream &os) const {
        return os << "GoalTimePlayer<" << CFloat(m_time_goal) << ">";
    }

    GoalTimePlayer(CookieClickerGame &game) : Player(game) {}

    void SetTimeGoal(float time_goal) {m_time_goal = time_goal;}

    int Play() const
    {
        float time_left = m_time_goal - m_game.GetTime();

        int best_unit = -1;
        float best_cookies = m_game.GetNumCookies() + m_game.GetCPS() * time_left;
        for (int unit = UNIT_CURSOR; unit <= UNIT_ANTIMATTER_CONDENSER; ++unit)
        {
            float cost = m_game.GetUnitPrice(static_cast<e_unit>(unit));
            float cps = m_game.GetUnitCPS(static_cast<e_unit>(unit));

            if (m_game.GetNumCookies() >= cost)
            {
                float cookies = m_game.GetNumCookies() - cost + (m_game.GetCPS() + cps) * time_left;
                if (cookies > best_cookies)
                {
                    best_cookies = cookies;
                    best_unit = unit;
                }
            }
        }

        return best_unit;
    }
}; // end of GoalTimePlayer

#endif // _PLAYER_H_

