#include "algorithm.hpp"

Algorithm::~Algorithm()
{}
void Algorithm::evolve_state()
{}
void Algorithm::setState(State* t_state)
{}

RECA::RECA()
{}

void RECA::setState(State* t_state){
    m_state = t_state;
    m_N = m_state->size();
    m_w = std::sqrt(m_N);
    m_replica.resize(m_N);
    for(int i=0; i<m_N; i++){
      m_replica[i] = (*m_state)[i];
    }
}

void RECA::evolve_state(){

  double R = rand0_1();

  for(int i=0; i<m_N; i++){
      (*m_state)[i] += R;
      if( (*m_state)[i] >= 1.0 ){
        (*m_state)[i] -= 1.0;
      }
  }

  int i = randN(m_N);

  swap(i);

  propigate(i);
  //std::cout << m_traversed.size() << std::endl;
  m_traversed.clear();
}
bool RECA::in_m_traversed(int i) {
  return std::find(m_traversed.begin(), m_traversed.end(), i) != m_traversed.end();
}
void RECA::swap(int j){
  double tmp = m_replica[j];
  m_replica[j] = (*m_state)[j];
  (*m_state)[j] = tmp;
}
void RECA::q_swap(int i , int j){
  double e1 = m_state->energy(i, j);
  swap(j);
  if ((1 - exp( (m_state->m_B)*(m_state->energy(i, j) - e1) )) > rand0_1()){
    propigate(j);
  }
  else{
    swap(j);
  }
}
void RECA::propigate(int i) {
  if( (i-m_w) >= 0 && !(in_m_traversed(i-m_w))){
    m_traversed.push_back(i-m_w);
    q_swap(i, i-m_w);
  }
  if( (i+m_w) < m_N && !(in_m_traversed(i+m_w))){
    m_traversed.push_back(i+m_w);
    q_swap(i, i+m_w);
  }
  if( ((i - 1) >= 0) && (((i - 1) % m_w) != (m_w-1)) && !(in_m_traversed(i-1)) ){
    m_traversed.push_back(i-1);
    q_swap(i, i-1);
  }
  if( ((i + 1) < m_N) && (((i + 1) % m_w) != 0) && !(in_m_traversed(i+1))){
    m_traversed.push_back(i+1);
    q_swap(i, i+1);
  }
}
