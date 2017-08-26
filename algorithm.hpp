#ifndef ALGORITHM_HPP
#define ALGORITHM_HPP

#include "state.hpp"
class State;
class Algorithm {
  public:
    virtual ~Algorithm();
    virtual void evolve_state();
    virtual void setState(State* t_state);
};
class RECA : public Algorithm {
  public:
    RECA();
    virtual void evolve_state();
    virtual void setState(State* t_state);
  private:
    State* m_state;
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
