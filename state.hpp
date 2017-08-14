#ifndef STATE_HPP
#define STATE_HPP

#include <algorithm>
#include <functional>
#include "couplings.hpp"
#include "utils.hpp"

namespace state {
  typedef std::function<double( int , int )> EnergyFunction_Ptr;
  class State{
    public:
      double m_B;
      State(int t_n, int t_B, EnergyFunction_Ptr t_f)
        : m_B(t_B), m_energy(t_f)
      {
        m_v.resize(t_n);
        std::generate( m_v.begin(), m_v.end(), utils::rand0_1 );
        m_replica = m_v;
        nsq = m_v.size();
        n = std::sqrt(nsq);
      }
      void print() {
        std::cout << "myvector contains:";
        for (std::vector<double>::iterator it=m_v.begin(); it!=m_v.end(); ++it)
          std::cout << ' ' << *it;
        std::cout << '\n';
      }
      double &operator[] (int i){
        return m_v[i];
      }
      int size(){
        return m_v.size();
      }
      void evolve_state() {

        double R = utils::rand0_1();
        for(int i=0; i<nsq; i++){
            m_v[i] += R;
            if( m_v[i] >= 1.0 ){
              m_v[i] -= 1.0;
            }
        }
        int i = utils::randN(nsq);

        double tmp = m_replica[i];
        m_replica[i] = m_v[i];
        m_v[i] = tmp;

        propigate(i);
        m_traversed.clear();
      }
    private:
      EnergyFunction_Ptr m_energy;
      std::vector<double> m_v;
      std::vector<double> m_replica;
      std::vector< int > m_traversed;
      int nsq;
      int n;

      bool in_m_traversed(int i) {
        return std::find(m_traversed.begin(), m_traversed.end(), i) != m_traversed.end();
      }
      void swap(int j){
        double tmp = m_replica[j];
        m_replica[j] = m_v[j];
        m_v[j] = tmp;
      }
      bool q_swap(int i , int j){
        double e1 = m_energy(i, j);
        swap(j);
        return ((1 - exp( m_B*(-1*e1 + m_energy(i, j)) )) > utils::rand0_1());
      }
      void propigate(int i) {
        if( (i-n) >= 0 && !(in_m_traversed(i-n))){
          m_traversed.push_back(i-n);
          if( q_swap(i, i-n) ){
            propigate(i-n);
          }
          else{
            swap(i-n);
          }
        }
        if( (i+n) < nsq && !(in_m_traversed(i+n))){
          m_traversed.push_back(i+n);
          if( q_swap(i, i+n) ){
            propigate(i+n);
          }
          else{
            swap(i+n);
          }
        }
        if( ((i - 1) >= 0) && (((i - 1) % n) != (n-1)) && !(in_m_traversed(i-1)) ){
          m_traversed.push_back(i-1);
          if( q_swap(i, i-1) ){
            propigate(i-1);
          }
          else{
            swap(i-1);
          }
        }
        if( ((i + 1) < nsq) && (((i + 1) % n) != 0) && !(in_m_traversed(i+1))){
          m_traversed.push_back(i+1);
          if( q_swap(i, i+1) ){
            propigate(i+1);
          }
          else{
            swap(i+1);
          }
        }
      }

  };
}
#endif
