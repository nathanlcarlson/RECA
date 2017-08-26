#include "state.hpp"

template<class Algo>
State<Algo>::State(int t_n, int t_B, EnergyFunction_Ptr t_f, Algo* t_algorithm)
  : m_B(t_B), m_energy(t_f)
{
  m_v.resize(t_n);
  seedRand(100);
  std::generate( m_v.begin(), m_v.end(), rand0_1 );

  //m_algorithm = new RECA();
  m_algorithm->setState(this);
}
template<class Algo>
double State<Algo>::energy(int i, int j){
  return m_energy(i,j);
}
template<class Algo>
void State<Algo>::print() {
  std::cout << "myvector contains:";
  for (std::vector<double>::iterator it=m_v.begin(); it!=m_v.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';
}
template<class Algo>
double &State<Algo>::operator[] (int i){
  return m_v[i];
}
template<class Algo>
int State<Algo>::size(){
  return m_v.size();
}
template<class Algo>
void State<Algo>::evolve_state() {
  m_algorithm->evolve_state();
}
