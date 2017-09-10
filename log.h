#ifndef _LOG_H_
#define _LOG_H_

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>

class Log
{
private:
    std::vector<std::string> m_entries;

public:
    Log() {}

    void Reset()
    {
        m_entries.clear();
    }

    void AddEntry(const std::string &entry)
    {
        m_entries.push_back(entry);
    }

    void Show() const
    {
        for (std::vector<std::string>::const_iterator it = m_entries.begin();
                it != m_entries.end(); ++it)
        {
            std::cout << *it << std::endl;
        }
    }
}; // end of class Log

#endif // _LOG_H_

