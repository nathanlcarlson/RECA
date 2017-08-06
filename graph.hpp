#ifndef GRAPH_HPP
#define GRAPH_HPP
#include <vector>
#include <unordered_set>
namespace graph{
  class GeneralGraph{
    public:
      GeneralGraph(int t_n)
      {
        m_v.resize(t_n);
      }
      void makeEdge(const int i,const int j){
        m_v[i].insert(j);
        m_v[j].insert(i);
      }
      void removeEdge(const int i, const int j){
        m_v[i].erase(j);
        m_v[j].erase(i);
      }
      void printGraph(){
        for(int i=0; i<m_v.size(); i++){
          if(m_v[i].size()==0){
            std::cout << i <<" has no edges";
          }
          else{
            for (const int x: m_v[i]){
              std::cout << i <<" has edges with: " << x;
            }
          }
          std::cout << std::endl;
        }
      }
    private:
      std::vector< std::unordered_set<int> > m_v;
  };
}
#endif
