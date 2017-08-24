#ifndef ALGORITHM_HPP
#define ALGORITHM_HPP

#include "state.hpp"
class State;
class Algorithm {
  public:
    virtual ~Algorithm();
    virtual void evolve_state();
    virtual void setState();
};
class RECA : public Algorithm {
  public:
    RECA(State* t_state);
  private:
    State* m_state;
    std::vector< double > m_replica;
    std::vector< int > m_traversed;
};
#endif
