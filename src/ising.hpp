#ifndef ISING_HPP
#define ISING_HPP

#include "state.hpp"
#include "couplings.hpp"
#include "discrete.hpp"

class JBonds: public StaticCouplings2D{
    public:
        JBonds(int _L)
        : StaticCouplings2D(_L)
        {
            square2DPeriodic();
        }
        double energy(int i, int j){
            return 1.0;
        }
};
class Ising: public State{
    private:
        JBonds J;
        Discrete up_down{-1.0, 1.0};
        int m_L;

    public:
        Ising(int _L, int _B)
        : State(_L, _B, up_down),
          J(_L),
          m_L(_L)
        {
            randomize_all();
        }

        double energy(int i, int j) {
            return  -1.0 * s(i) * s(j);
        }

        JBonds::const_iterator neighbors_begin(int i){
            return J.begin(i);
        }
        JBonds::const_iterator neighbors_end(int i){
            return J.end(i);
        }
};
#endif
