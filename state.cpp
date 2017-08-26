#include "state.hpp"

State::State(int t_n, int t_B, EnergyFunction_Ptr t_f, int t_A)
  : m_B(t_B), m_energy(t_f)
{
  m_v.resize(t_n);
  seedRand(100);
  std::generate( m_v.begin(), m_v.end(), rand0_1 );

  m_algorithm = new RECA( this );
}
void State::print() {
  std::cout << "myvector contains:";
  for (std::vector<double>::iterator it=m_v.begin(); it!=m_v.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';
}
double &State::operator[] (int i){
  return m_v[i];
}
int State::size(){
  return m_v.size();
}
void State::evolve_state() {
  m_algorithm->evolve_state();
}
