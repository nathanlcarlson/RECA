#ifndef COUPLINGS_HPP
#define COUPLINGS_HPP

#include <map>
#include <functional>
#include <tuple>
#include "utils.hpp"

namespace couplings{
  typedef std::function<double( utils::node, utils::node )> EnergyFunction_Ptr;
  class GeneralCouplings {
    public:
      GeneralCouplings(int t_n, EnergyFunction_Ptr f)
        : m_size(t_n), energy(f)
      {
      }

      bool are_coupled(const int i, const int j){
        return m_map.find(std::make_pair(i,j)) != m_map.end();
      }
      double &operator() (int i, int j){
        return m_map[std::make_pair(i,j)];
      }
      void square2D( bool periodic = true ){
        int w = std::sqrt(m_size);
        int j;
        int k;
        int x;
        int y;
        utils::node inode;
        utils::node jnode;
        if (periodic) {
          for(int i = 0; i < m_size; i++){
            x = i%w;
            y = i/w;
            j = w*y + utils::mod( i+1, w );
            m_map[std::make_pair(i,j)] = energy(
              utils::make_node(x, y, 0, 0), utils::make_node(j%w, j/w, 0, 0));
            j = w*(y) + utils::mod( i-1, w );
            m_map[std::make_pair(i,j)] = energy(
              utils::make_node(x, y, 0, 0), utils::make_node(j%w, j/w, 0, 0));
            j = utils::mod( i+w, m_size );
            m_map[std::make_pair(i,j)] = energy(
              utils::make_node(x, y, 0, 0), utils::make_node(j%w, j/w, 0, 0));
            j = utils::mod( i-w, m_size );
            m_map[std::make_pair(i,j)] = energy(
              utils::make_node(x, y, 0, 0), utils::make_node(j%w, j/w, 0, 0));
          }
        }
        else{
          for(int i = 0; i < m_size; i++){
            x = i%w;
            y = i/w;
            if( (i+1)%w != 0 ){
              j = w*y+utils::mod( i+1, w);
              m_map[std::make_pair(i,j)] = energy(
                utils::make_node(x, y, 0, 0), utils::make_node(j%w, j/w, 0, 0));
            }
            if( x != 0 ){
              j = w*y+utils::mod( i-1, w);
              m_map[std::make_pair(i,j)] = energy(
                utils::make_node(x, y, 0, 0), utils::make_node(j%w, j/w, 0, 0));
            }
            if( i+w < m_size ){
              j = utils::mod( i+w, m_size);
              m_map[std::make_pair(i,j)] = energy(
                utils::make_node(x, y, 0, 0), utils::make_node(j%w, j/w, 0, 0));
            }
            if( i-w >= 0 ){
              j = utils::mod( i-w, m_size);
              m_map[std::make_pair(i,j)] = energy(
                utils::make_node(x, y, 0, 0), utils::make_node(j%w, j/w, 0, 0));
            }
          }
        }
      }
      void print(){
        std::map<std::pair<int, int>, double>::iterator it = m_map.begin();
        std::cout << "Couplings:\n";
        for (it=m_map.begin(); it!=m_map.end(); ++it)
          std::cout << "(" << it->first.first << ", " << it->first.second << ")" << " => " << it->second << '\n';
      }
    private:
      EnergyFunction_Ptr energy;
      int m_size;
      std::map<std::pair<int, int>, double> m_map;
  };
}
#endif
