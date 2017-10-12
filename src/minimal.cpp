#include <iostream>
#include <random>
#include <vector>
#include <map>
#include <algorithm>
#include <math.h>


#define N 2
#define L 2

int main(int argc, char **argv) {

  // Total number of states
  int N_states = pow(N, 2*L);
  int N_pri = pow(N, L);
  // Minimum number of bits required to represent N spins
  int bits = ceil(log(N)/log(2));
  // State
  std::vector< int > s;
  // Replica
  std::vector< int > u;
  s.resize(L);
  u.resize(L);
  
  s = {0, 0};
  u = {0, 0}; 
  // Set to store unique_states
  std::vector< int > states_histogram;
  std::vector< int > pri_histogram;
  
  states_histogram.resize(N_states);
  pri_histogram.resize(N_pri);
  std::cout.setf(std::ios_base::fixed, std::ios_base::floatfield);
  std::cout.precision(2);
  int count = 0;
  while( true ){
    // Metropolis step
    if (rand()%100 < 101){
      int i = rand()%L;
      int temp = s[i];
      
      s[i] = rand()%N;
      int p = 1;
      if (u[0] == u[1]) { 
        p *= 2;  
      }
      if (s[0] == s[1]) {
        p *= 2;
      }
      if (rand() % 4 + 1 > p) {
        s[i] = temp;
      }
      
    }
    else {
      int R = rand()%N;
      for (int i = 0; i < L; i++)
      {
	      u[i] = (u[i] + R)%N;
      }

      // Swap between state and replica
      int i = rand()%L;
      int t = s[i];
      s[i] = u[i];
      u[i] = t;
    }

    // Check states reached
    int state = 0;
    int pri = 0;
    // Create bit string
    for (int j = 0; j < L; j++)
    {
      //std::cout << (u[j] << j*bits) << ' ';
	    state += s[j] << j*bits;
	    pri += s[j] << j*bits;
    }
    //std::cout << std::endl;
    for (int j = L; j < 2*L; j++)
    {
      //std::cout << (s[j-L] << j*bits) << ' ';
	    state += u[j-L] << j*bits;
	    
    }
    int weight = 1;
    
    
    states_histogram[ state ] += weight;
    pri_histogram[ pri ] += weight;
    
    if(count % 2000 == 0){
      std::cout << "\nTotal system\n";
      double max = *max_element(states_histogram.begin(), states_histogram.end());
      for (int j = 0; j < N_states; j++) std::cout << "   " << j << " : " << states_histogram[j]/(max) << "\n";
      std::cout << "\nPrimary system\n";
      max = *max_element(pri_histogram.begin(), pri_histogram.end());
      for (int j = 0; j < N_pri; j++) std::cout << "   " << j << " : " << pri_histogram[j]/(max) << "\n";
    }

    count ++;
  }
  return 0;
	
}
