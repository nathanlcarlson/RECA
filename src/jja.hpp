#ifndef JJA_HPP
#define JJA_HPP

#include "state.hpp"
#include "couplings.hpp"
#include "continuous.hpp"

class ABonds: public StaticCouplings2D{
    public:
        ABonds(int _L)
        : StaticCouplings2D(_L)
        {
            square2DAperiodic();
        }
        double energy(int i, int j){
            return x(i)*y(j) - x(j)*y(i);
        }
};
class JJA: public State{

    private:
        ABonds A;
        Continuous clock;
        double m_L;

    public:
        JJA(int _L, int _B)
        : clock(0, 2*M_PI),
          State(_L, _B, clock),
          A(_L),
          m_L(_L)
        {
            randomize_all();
            A.scale_all(-2.0*M_PI/(10.0*(std::sqrt(_L)-1)));
            // A.print();
        }

        double energy(int i, int j) {
            return -1*cos(s(i)-s(j)-A.get(i,j));
        }
        void metrics(std::vector<double>& out) {
            out.clear();
            out.resize(4);
            double E = 0;
            double Mx = 0;
            double My = 0;
            double Mz = 0;
            // double Mc = 0;
            // double Ms = 0;
            for(int i = 0; i < m_L; i++) {
                for (auto neighbor = neighbors_begin(i); neighbor != neighbors_end(i); ++neighbor) {
                    int j = *neighbor;
                    if( i < j) {
                        E += energy(i,j);
                        Mz += 0.5*A.energy(i,j)*sin(s(i)-s(j)-A.get(i,j));
                    }
                }
                Mx += cos(s(i));
                My += sin(s(i));
            }
            out[0] = E;
            out[1] = Mx;
            out[2] = My;
            out[3] = Mz;
        }
        ABonds::const_iterator neighbors_begin(int i){
            return A.begin(i);
        }
        ABonds::const_iterator neighbors_end(int i){
            return A.end(i);
        }
};

#endif