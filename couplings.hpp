#ifndef COUPLINGS_HPP
#define COUPLINGS_HPP

#include <map>
#include "utils.hpp"

namespace couplings{

  class GeneralCouplings {
    public:
      GeneralCouplings(int t_n)
        : m_size(t_n)
      {
      }
      void couple(const int i, const int j, const double h){
        m_map.insert(std::make_pair(std::make_pair(i,j), h));
        m_map.insert(std::make_pair(std::make_pair(j,i), h));
      }
      void decouple(const int i, const int j){
        m_map.erase(std::make_pair(i,j));
        m_map.erase(std::make_pair(j,i));
      }
      bool are_coupled(const int i, const int j){
        return m_map.find(std::make_pair(i,j)) != m_map.end();
      }
      double &operator() (int i, int j){
        return m_map[std::make_pair(i,j)];
      }
      void print(){
        std::map<std::pair<int, int>, double>::iterator it = m_map.begin();
        std::cout << "Couplings:\n";
        for (it=m_map.begin(); it!=m_map.end(); ++it)
          std::cout << "(" << it->first.first << ", " << it->first.second << ")" << " => " << it->second << '\n';
      }
    protected:
      int m_size;
      std::map<std::pair<int, int>, double> m_map;
  };
      }
  };
}
#endif
