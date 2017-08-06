#ifndef FIELDS_HPP
#define FIELDS_HPP

#include <vector>

namespace fields {
  class UniformField {
    public:
      UniformField( double t_h )
        : m_h(t_h)
      {}
      double &operator[] (int v)
      {
        return m_h;
      };
    private:
      double m_h { 0 };
  };
  class GeneralField {
    public:
      GeneralField( int t_n = 0, double t_h = 0 )
        : m_n(t_n)
      {
        m_v.resize(t_n);
        for(int i = 0; i < t_n; i++){
          m_v[i] = t_h;
        }
      }
      double &operator[] (int i)
      {
        return m_v[i];
      };
    private:
      int m_n { 0 };
      std::vector<double> m_v;
  };
}
#endif
