#ifndef COUPLINGS_HPP
#define COUPLINGS_HPP

#include <vector>
namespace couplings{
  class GeneralCouplings {
    public:
      GeneralCouplings(int t_n)
      {
        m_v.resize(t_n);
      }
      double &operator[] (int i, int j){
        return m_v[i][j];
      }
    private:
      std::vector< std::vector<double> > m_v;
  };
}
#endif
