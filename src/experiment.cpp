#include <algorithm>
#include <vector>
#include <set>
#include <cmath>
#include <iostream>
#include <time.h>

// Number of spins a node can have
int N = 8;
// Number of nodes in the state, thus including the replica there are 2L nodes
int L = 3;
// Total number of states
int N_states = pow(N, 2*L);
int N_pri = pow(N, L);
// Minimum number of bits required to represent N spins
int bits = ceil(log(N)/log(2));

int rand0_N(){
  return rand()%N;
}
int main(int argc, char **argv){
  // State
  std::vector< int > s;
  // Replica
  std::vector< int > u;
  // Set to store unique_states
  std::set< int > unique_states;
  std::set< int > unique_rep;
  std::set< int > unique_pri;

  s.resize(L);
  u.resize(L);
  // Seed random generator
  srand( time(NULL) );

  // Populate state and replica
  std::generate( s.begin(), s.end(), rand0_N );
  std::generate( u.begin(), u.end(), rand0_N );
  // Method of occasionally pausing execution
  int count = 0;
  int steps = 10000;

  while( true ){
    // Metropolis step
    // if (rand()%100 < 25){
    //   s[rand()%L] = rand()%N;
    // }
    // Global transformation
    int R = rand()%(N-1);
		for (int i = 0; i < L; i++)
		{
			u[i] = (u[i] + R)%N;
		}

    // Swap between state and replica
		int i = rand()%L;
		int t = s[i];
    s[i] = u[i];
    u[i] = t;

    // Grow cluster?
    // int n = rand()%L;
    // for (int j = 0; j < n; j++){
    //   t = s[(i + j)%L];
    //   s[(i + j)%L] = u[(i + j)%L];
    //   u[(i + j)%L] = t;
    // }

    // Check states reached
    int state = 0;
    int pri = 0;
    int rep = 0;
    // Create bit string
    for (int j = 0; j < L; j++)
		{
      //std::cout << (u[j] << j*bits) << ' ';
			state += u[j] << j*bits;
      rep += u[j] << j*bits;
		}
    //std::cout << std::endl;
    for (int j = L; j < 2*L; j++)
		{
      //std::cout << (s[j-L] << j*bits) << ' ';
			state += s[j-L] << j*bits;
      pri += s[j-L] << j*bits;
		}
    // The set data type makes sure all elements are unique
    unique_states.insert( state );
    unique_pri.insert( pri );
    unique_rep.insert( rep );

    // Information about the system
    std::cout << std::endl;
    std::cout << "Number of states: " << unique_states.size() << '\n';
    // std::cout << "unique_states contains:";
    // for (auto it=unique_states.begin(); it!=unique_states.end(); ++it)
    //   std::cout << ' ' << *it;
    // std::cout << '\n';
    if(unique_rep.size() == N_pri && unique_pri.size() == N_pri){
      std::cout << "All pri and rep states " << N_pri << std::endl;
      //getchar();
    }
    if(unique_states.size() == N_states){
      std::cout << "All states " << N_states << std::endl;
      return 0;
    }
    // Wait for input
    count ++;
    if (count == steps){
      std::cout << "Press enter to continue\n";
      getchar();
      count = 0;
    }
  }
}
