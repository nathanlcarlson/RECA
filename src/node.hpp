#ifndef NODE_HPP
#define NODE_HPP

#include <vector>
#include <algorithm>
#include "utils.hpp"

class Node {

  private:
    int m_N;
    bool m_continuous;
    int m_Min;
    int m_Max;
    std::vector<double> m_values;
    double m_shift;

  public:
    // Assume 0 to max, exclusive, continuous
    Node(double _max)
      : m_Max(_max)
    {
      m_Min = 0;
      m_continuous = true;
    }

    // Min to max, exlusive, continuous
    Node(double _min, double _max)
      : m_Min(_min), m_Max(_max)
    {
      m_continuous = true;
    }

    // Min to max, exlusive, stepwise, discrete
    Node(double _min, double _max, double _step)
      : m_Min(_min), m_Max(_max)
    {
      m_continuous = false;
      double v = _min;
      while (v < _max) {
        m_values.push_back(v);
        v += _step;
      }
      m_N = m_values.size();
    }

    // Values given
    Node(std::vector<double>& _v)
    {
      m_values = _v;
      m_N = m_values.size();
    }

    // Select random value
    double random_value() {
      if(m_continuous) {
        return rand0_1()*(m_Max - m_Min) + m_Min;
      }
      else{
        return m_values[randN(m_N)];
      }
    }

    // Shift a value
    double shifted_value(double _cur){
      if(m_continuous){
        double v = _cur + m_shift;
        if(v > m_Max){
          v -= (m_Max - m_Min);
        }
        return v;
      }
      else{
        int i = std::find(m_values.begin(), m_values.end(), _cur) - m_values.begin();

        return m_values[(i + (int)m_shift)%m_N];
      }
    }

    // Sets the value of the next shift
    void set_shift() {
      if(m_continuous){
        m_shift = rand0_1()*(m_Max - m_Min);
      }
      else{
        m_shift = 1 + randN(m_N - 1);
      }
    }

    // TODO Make cyclic and not cyclic versions of this
    std::vector<double> getRGB(double _v) {
      double r;
      if(m_continuous){
        r = (_v-m_Min)/(m_Max-m_Min);
        return { hueToRGB(r + 0.3333) , hueToRGB(r) , hueToRGB(r - 0.3333)};
      }
      else{
        int i = std::find(m_values.begin(), m_values.end(), _v) - m_values.begin();
        r = i/(double)(m_N-1);
        return { r , r, r};
      }

    }
};


#endif