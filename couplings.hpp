#ifndef COUPLINGS_HPP
#define COUPLINGS_HPP

#include <unordered_map>
#include <functional>
#include "utils.hpp"

namespace couplings{
  typedef std::function<double( utils::node, utils::node )> CouplingEnergyFunction_Ptr;
  class StaticCouplings2D {
    public:
      StaticCouplings2D(int t_n, CouplingEnergyFunction_Ptr t_f)
        : m_size(t_n), m_energy(t_f)
      {
        m_map.resize(t_n);
      }

      double &operator() (int t_i, int t_j){
        return m_map[t_i][t_j];
      }
      void square2D( bool periodic = true ){
        int w = std::sqrt(m_size);
        int j;
        int x;
        int y;
        if (periodic) {
          for(int i = 0; i < m_size; i++){
            x = i%w;
            y = i/w;
            j = w*y + utils::mod( i+1, w );
            m_map[i][j] = m_energy(
              utils::make_node(x, y, 0),
              utils::make_node(j%w, j/w, 0)
            );
            j = w*(y) + utils::mod( i-1, w );
            m_map[i][j] = m_energy(
              utils::make_node(x, y, 0),
              utils::make_node(j%w, j/w, 0)
            );
            j = utils::mod( i+w, m_size );
            m_map[i][j] = m_energy(
              utils::make_node(x, y, 0),
              utils::make_node(j%w, j/w, 0));
            j = utils::mod( i-w, m_size );
            m_map[i][j] = m_energy(
              utils::make_node(x, y, 0),
              utils::make_node(j%w, j/w, 0));
          }
        }
        else{
          for(int i = 0; i < m_size; i++){
            x = i%w;
            y = i/w;
            if( (i+1)%w != 0 ){
              j = w*y+utils::mod( i+1, w);
              m_map[i][j] = m_energy(
                utils::make_node(x, y, 0),
                utils::make_node(j%w, j/w, 0));
            }
            if( x != 0 ){
              j = w*y+utils::mod( i-1, w);
              m_map[i][j] = m_energy(
                utils::make_node(x, y, 0),
                utils::make_node(j%w, j/w, 0));
            }
            if( i+w < m_size ){
              j = utils::mod( i+w, m_size);
              m_map[i][j] = m_energy(
                utils::make_node(x, y, 0),
                utils::make_node(j%w, j/w, 0));
            }
            if( i-w >= 0 ){
              j = utils::mod( i-w, m_size);
              m_map[i][j] = m_energy(
                utils::make_node(x, y, 0),
                utils::make_node(j%w, j/w, 0));
            }
          }
        }
      }
      void print(){
        int c = 0;
        for (std::vector<std::unordered_map<int, double>>::iterator it = m_map.begin() ;  it != m_map.end(); ++it){
          for (auto& x: (*it)) {
            std::cout << "( "<< c << ", " << x.first << " ) => " << x.second << std::endl;
          }
          c++;
        }
      }
    private:
      CouplingEnergyFunction_Ptr m_energy;
      int m_size;
      std::vector< std::unordered_map<int, double> > m_map;
  };
}
#endif
