#ifndef DISCRETE_HPP
#define DISCRETE_HPP
//TODO Split into header/implementation
#include <vector>
#include <unordered_map>
#include "utils.hpp"
#include "node.hpp"

class Discrete: public Node {

  private:
    int m_N;
    std::vector<double> m_values;
    std::vector<std::vector<double>> m_shift_vals;
    std::unordered_map<double, int> m_loc;

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

    // Min to max, exlusive, stepwise, discrete
    Discrete(double _min, double _max, double _step)
    {
      while (_min < _max) {
        m_values.push_back(_min);
        _min += _step;
      }
      m_N = m_values.size();
      populate_shift_info();
    }

    // Values given, discrete
    Discrete(std::vector<double>& _v)
    {
      m_values = _v;
      m_N = m_values.size();
      populate_shift_info();
    }

    // Select random value
    double random_value() {
      return m_values[randN(m_N)];
    }

    // Shift a value
    double shifted_value(double _cur, double _shift){
      return m_shift_vals[m_loc[_cur]][mod((int)_shift, m_N)];
    }
    double shifted_value(double _cur){
      return shifted_value(_cur, get_shift());
    }
    double get_shift(){
      return 1+randN(m_N-1);
    }

    // TODO Make cyclic and not cyclic versions of this
    void getRGB(double _v, std::vector<double>& _rgb) {
      _v = m_loc[_v]/(double)(m_N-1);
      _rgb[0] = _v;
      _rgb[1] = _v;
      _rgb[2] = _v;
    }
};
#endif
