#include "algorithm.hpp"

RECA::RECA(State *t_state)
  : m_state(t_state)
{}
void RECA::evolve_state(){

  double R = rand0_1();
  for(int i=0; i<nsq; i++){
      m_v[i] += R;
      if( m_v[i] >= 1.0 ){
        m_v[i] -= 1.0;
      }
  }
  int i = randN(nsq);

  double tmp = m_replica[i];
  m_replica[i] = m_state.m_v[i];
  m_state.m_v[i] = tmp;

  //propigate(i);
  //std::cout << m_traversed.size() << std::endl;
  m_traversed.clear();
}
