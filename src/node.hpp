#ifndef NODE_HPP
#define NODE_HPP

#include <vector>
#include <algorithm>
#include <unordered_map>
#include "utils.hpp"

class Node {

  private:
    int m_N;
    bool m_continuous;
    int m_Min;
    int m_Max;
    std::vector<double> m_values;
    std::vector<std::vector<double>> m_shift_vals;
    std::unordered_map<double, int>  m_loc;
    double m_shift;

    void populate_shift_info(){
      m_shift_vals.resize(m_N);
      for(int i=0; i<m_N; ++i){
        m_loc[m_values[i]] = i;
        for(int j=0; j<m_N; ++j){
          m_shift_vals[i].push_back(m_values[(i+j)%m_N]);
        }
      }
    }
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
      populate_shift_info();
    }

    // Values given, discrete
    Node(std::vector<double>& _v)
    {
      m_values = _v;
      m_N = m_values.size();
      populate_shift_info();
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
    double shifted_value(double _cur, double _shift = 0){
      if (_shift == 0){
        _shift = m_shift;
      }
      if(m_continuous){
        double v = _cur + _shift;
        if(v > m_Max){
          v -= (m_Max - m_Min);
        }
        if(v < m_Min){
          v += (m_Max - m_Min);
        }
        return v;
      }
      else{
        return m_shift_vals[m_loc[_cur]][mod((int)_shift, m_N)];
      }
    }

    // Sets the value of the next shift
    double set_shift() {
      if(m_continuous){
        m_shift = rand0_1()*(m_Max - m_Min);
      }
      else{
        m_shift = 1 + randN(m_N - 1);
      }
      return m_shift;
    }

    // TODO Make cyclic and not cyclic versions of this
    std::vector<double> getRGB(double _v) {
      double r;
      if(m_continuous){
        r = (_v-m_Min)/(m_Max-m_Min);
        return { hueToRGB(r + 0.3333) , hueToRGB(r) , hueToRGB(r - 0.3333)};
      }
      else{
        int i = m_loc[_v];
        r = i/(double)(m_N-1);
        return { r , r, r};
      }

    }
};


#endif
