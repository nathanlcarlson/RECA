#ifndef CONTINUOUS_HPP
#define CONTINUOUS_HPP

#include <vector>
#include "utils.hpp"
#include "node.hpp"

class Continuous: public Node {

  private:
    double m_Max;
    double m_Min;
    double m_range;

  public:
    // Assume 0 to max, exclusive, continuous
    Continuous(double _max)
      : m_Max(_max)
    {
      m_Min = 0;
      m_range = m_Max - m_Min;
    }

    // Min to max, exlusive, continuous
    Continuous(double _min, double _max)
      : m_Min(_min), m_Max(_max)
    {
      m_range = m_Max - m_Min;
    }

    // Select random value
    double random_value() {
      return rand0_1()*m_range + m_Min;
    }

    // Shift a value
    double shifted_value(double _cur, double _shift){
      double v = _cur + _shift;
      if(v > m_Max){
        v -= m_range;
      }
      else if(v < m_Min){
        v += m_range;
      }
      return v;
    }
    double shifted_value(double _cur){
      return shifted_value(_cur, get_shift());
    }
    double get_shift(){
      return random_value();
    }

    // TODO Make cyclic and not cyclic versions of this
    void getRGB(double _v, std::vector<double>& _rgb) {
      double r = (_v-m_Min)/(m_Max-m_Min);
      _rgb[0] = hueToRGB(r + 0.3333);
      _rgb[1] = hueToRGB(r);
      _rgb[2] = hueToRGB(r - 0.3333);
    }
};


#endif
