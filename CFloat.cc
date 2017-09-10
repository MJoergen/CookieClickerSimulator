#include <iomanip>
#include "CFloat.h"

static float round(float val)
{
    return (((int)(val*10.0 + 0.5)) / 10.0);
}

std::ostream& operator <<(std::ostream &os, const CFloat &cfloat)
{
    if (cfloat.m_val < 1.0e3) {
        return os << std::setprecision(3) << (cfloat.m_val);
    } else if (cfloat.m_val < 1.0e6) {
        return os << std::setprecision(3) << (cfloat.m_val / 1.0e3) << "k";
    } else if (cfloat.m_val < 1.0e9) {
        return os << std::setprecision(3) << (cfloat.m_val / 1.0e6) << "M";
    } else {
        return os << std::setprecision(3) << (cfloat.m_val / 1.0e9) << "G";
    }
} // end of operator <<

