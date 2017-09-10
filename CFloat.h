#ifndef _CFLOAT_H_
#define _CFLOAT_H_

#include <ostream>

class CFloat
{
  friend std::ostream& operator <<(std::ostream &os, const CFloat &cfloat);

public:
    CFloat(float val) : m_val(val) {}

private:
    float m_val;
}; // end of class CFloat

#endif // _CFLOAT_H_

