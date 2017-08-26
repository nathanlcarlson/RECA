#ifndef ALGORITHM_HPP
#define ALGORITHM_HPP

#include "state.hpp"
template<class Algo>
class State;

class RECA {
  public:
    RECA();
    void evolve_state();
    void setState(State<RECA>* t_state);
  private:
    State<RECA>* m_state;
    std::vector< double > m_replica;
    std::vector< int > m_traversed;
    int m_N;
    int m_w;
    bool in_m_traversed(int i);
    void swap(int j);
    void q_swap(int i , int j);
    void propigate(int);
};
#endif
