#ifndef _COOKIECLICKER_H_
#define _COOKIECLICKER_H_

#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <string>

#include "CFloat.h"
#include "log.h"

#define COST_CURSOR                   15.0
#define COST_GRANDMA                 100.0
#define COST_FARM                      1.1e3
#define COST_MINE                     12.0e3
#define COST_FACTORY                 130.0e3
#define COST_BANK                      1.4e6
#define COST_TEMPLE                   20.0e6
#define COST_WIZARD_TOWER            330.0e6
#define COST_SHIPMENT                  5.1e9
#define COST_ALCHEMY_LAB              75.0e9
#define COST_PORTAL                    1.0e12
#define COST_TIME_MACHINE             14.0e12
#define COST_ANTIMATTER_CONDENSER    170.0e12
#define COST_PRISM                     2.1e15
#define COST_CHANCEMAKER              26.0e15

#define COST_INCREASE 1.15

#define CPS_CURSOR                     0.1
#define CPS_GRANDMA                    1.0
#define CPS_FARM                       8.0
#define CPS_MINE                      47.0
#define CPS_FACTORY                  260.0
#define CPS_BANK                       1.4e3
#define CPS_TEMPLE                     7.8e3
#define CPS_WIZARD_TOWER              44.0e3
#define CPS_SHIPMENT                 260.0e3
#define CPS_ALCHEMY_LAB                1.6e6
#define CPS_PORTAL                    10.0e6
#define CPS_TIME_MACHINE              65.0e6
#define CPS_ANTIMATTER_CONDENSER     430.0e6
#define CPS_PRISM                      2.9e9
#define CPS_CHANCEMAKER               21.0e9

typedef enum {
    UNIT_CURSOR,
    UNIT_GRANDMA,
    UNIT_FARM,
    UNIT_MINE,
    UNIT_FACTORY,
    UNIT_BANK,
    UNIT_TEMPLE,
    UNIT_WIZARD_TOWER,
    UNIT_SHIPMENT,
    UNIT_ALCHEMY_LAB,
    UNIT_PORTAL,
    UNIT_TIME_MACHINE,
    UNIT_ANTIMATTER_CONDENSER,
    UNIT_PRISM,
    UNIT_CHANCEMAKER,
    NUM_UNITS
} e_unit;

class CookieClickerGame
{
private:
    std::vector<float> m_units;
    std::vector<float> m_prices;
    std::vector<float> m_cps;
    std::vector<std::string> m_str_unit;

    float m_cookies;
    float m_cookiesPerSecond;
    float m_time;

    Log m_log;

public:
    CookieClickerGame()
    {
        Reset();
    }

    void Reset() {
        m_cookies = 0.0;
        m_cookiesPerSecond = 0.0;
        m_time = 0.0;

        m_units.clear();
        for (int i=0; i<NUM_UNITS; ++i) {
            m_units.push_back(0.0);
        }

        m_str_unit.clear();
        m_str_unit.push_back("Cursor              ");
        m_str_unit.push_back("Grandma             ");
        m_str_unit.push_back("Farm                ");
        m_str_unit.push_back("Mine                ");
        m_str_unit.push_back("Factory             ");
        m_str_unit.push_back("Bank                ");
        m_str_unit.push_back("Temple              ");
        m_str_unit.push_back("Wizard Tower        ");
        m_str_unit.push_back("Shipment            ");
        m_str_unit.push_back("Alchemy Lab         ");
        m_str_unit.push_back("Portal              ");
        m_str_unit.push_back("Time Machine        ");
        m_str_unit.push_back("Antimatter Condenser");
        m_str_unit.push_back("Prism               ");
        m_str_unit.push_back("Chancemaker         ");

        m_prices.clear();
        m_prices.push_back(COST_CURSOR);
        m_prices.push_back(COST_GRANDMA);
        m_prices.push_back(COST_FARM);
        m_prices.push_back(COST_MINE);
        m_prices.push_back(COST_FACTORY);
        m_prices.push_back(COST_BANK);
        m_prices.push_back(COST_TEMPLE);
        m_prices.push_back(COST_WIZARD_TOWER);
        m_prices.push_back(COST_SHIPMENT);
        m_prices.push_back(COST_ALCHEMY_LAB);
        m_prices.push_back(COST_PORTAL);
        m_prices.push_back(COST_TIME_MACHINE);
        m_prices.push_back(COST_ANTIMATTER_CONDENSER);
        m_prices.push_back(COST_PRISM);
        m_prices.push_back(COST_CHANCEMAKER);

        m_cps.clear();
        m_cps.push_back(CPS_CURSOR);
        m_cps.push_back(CPS_GRANDMA);
        m_cps.push_back(CPS_FARM);
        m_cps.push_back(CPS_MINE);
        m_cps.push_back(CPS_FACTORY);
        m_cps.push_back(CPS_BANK);
        m_cps.push_back(CPS_TEMPLE);
        m_cps.push_back(CPS_WIZARD_TOWER);
        m_cps.push_back(CPS_SHIPMENT);
        m_cps.push_back(CPS_ALCHEMY_LAB);
        m_cps.push_back(CPS_PORTAL);
        m_cps.push_back(CPS_TIME_MACHINE);
        m_cps.push_back(CPS_ANTIMATTER_CONDENSER);
        m_cps.push_back(CPS_PRISM);
        m_cps.push_back(CPS_CHANCEMAKER);

        m_log.Reset();
    }

#define EPS 0.00000001

    void Buy(e_unit unit) {
        if (m_cookies + EPS >= m_prices[unit]) {
            std::stringstream buffer;
            buffer << "At " << std::setw(5) << m_time;
            buffer << " with " << std::setw(5) << (unsigned long) (m_cookies + 0.5) << " cookies";
            buffer << " (" << std::fixed << std::setprecision(1) << m_cookiesPerSecond << " CPS)";
            buffer << " buy one " << m_str_unit[unit];
            buffer << " for " << m_prices[unit];

            m_log.AddEntry(buffer.str());
            m_cookies -= m_prices[unit];
            m_prices[unit] *= COST_INCREASE;
            m_cookiesPerSecond += m_cps[unit];
            m_units[unit] ++;
        } else {
            std::cerr << "NOT ENOUGH MONEY!" << std::endl;
            std::cout << std::endl;
        }
    }

    void Click(float numClicks = 1.0) {
        m_cookies += numClicks;
    }

    void PassTime(float seconds) {
        m_cookies += m_cookiesPerSecond * seconds;
        m_time += seconds;
    }

    void Show() const {
        std::cout << "Time    : " << CFloat(m_time) << std::endl;
        std::cout << "Cookies : " << CFloat(m_cookies) << std::endl;
        std::cout << "CPS     : " << CFloat(m_cookiesPerSecond) << std::endl;
        for (int unit = UNIT_CURSOR; unit <= UNIT_ANTIMATTER_CONDENSER; ++unit) {
            std::cout << m_str_unit[unit] << " -> " << std::setw(3) << CFloat(m_units[unit]);
            std::cout << "  (" << CFloat(m_prices[unit]) << ") or (";
            std::cout << CFloat(m_prices[unit]/m_cps[unit]) << " per CPS)" << std::endl;
        }
        std::cout << std::endl;
    }

    void ShowLog() const {
        m_log.Show();
    }

    float GetUnitCPS(e_unit unit) const {
        return m_cps[unit];
    }

    float GetUnitPrice(e_unit unit) const {
        return m_prices[unit];
    }

    float GetUnitCount(e_unit unit) const {
        return m_units[unit];
    }

    float GetTime() const {
        return m_time;
    }

    float GetCPS() const {
        return m_cookiesPerSecond;
    }

    float GetNumCookies() const {
        return m_cookies;
    }
}; // end of class CookieClickerGame

#endif // _COOKIECLICKER_H_

