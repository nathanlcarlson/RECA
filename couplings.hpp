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
  class SquareCouplings: public GeneralCouplings {
    public:
      SquareCouplings(int t_n, const double h, bool periodic = true) : GeneralCouplings(t_n){
        int w = std::sqrt(m_size);
        if (periodic) {
          for(int i = 0; i < m_size; i++){
            this->couple(i, w*(i/w)+utils::mod( i+1, w), h);
            this->couple(i, w*(i/w)+utils::mod( i-1, w), h);
            this->couple(i, utils::mod( i+w, m_size), h);
            this->couple(i, utils::mod( i-w, m_size), h);
          }
        }
        else{
          for(int i = 0; i < m_size; i++){
            if( (i+1)%w != 0 ){
              this->couple(i, w*(i/w)+utils::mod( i+1, w), h);
            }
            if( i%w != 0 ){
              this->couple(i, w*(i/w)+utils::mod( i-1, w), h);
            }
            if( i+w < m_size ){
              this->couple(i, utils::mod( i+w, m_size), h);
            }
            if( i-w > 0 ){
              this->couple(i, utils::mod( i-w, m_size), h);
            }
          }
        }
      }
  };
}
#endif
