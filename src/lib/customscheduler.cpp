
#include <ecto/customscheduler.hpp>

using namespace ecto;
using namespace std;


CustomSchedulerSBR::CustomSchedulerSBR(plasm_ptr p): scheduler(p)
{}

CustomSchedulerSBR::~CustomSchedulerSBR()
{}

bool CustomSchedulerSBR::execute(vector<std::string> id)
{  
  NodesVector transit;
  for(std::vector<std::string>::iterator it = id.begin(); it != id.end(); it++)
  {
    transit.push_back(findNode(*it));
  }
  
  compute_stack(transit);
  return this->execute(1);
}


void CustomSchedulerSBR::compute_stack(NodesVector vec)
{
  NodesVector sub = getSubgraph(vec), localStack;

  DepthType m = getDepthMap(sub);
  int i = 0;
  localStack=getDepthNodes(m,i);
  stack_.clear();
  while(!localStack.empty())
  {
    stack_.resize(stack_.size() + localStack.size());
    copy_backward(localStack.begin(), localStack.end(), stack_.end());
    i++;
    localStack=getDepthNodes(m,i);
  }
  reverse(stack_.begin(), stack_.end());
  
//   cout<<"Compute"<<endl;
//   for(auto a = stack_.begin(); a != stack_.end(); a++)
//     cout<<graph_[*a]->cell()->name()<<endl;
  
}

graph::graph_t::vertex_descriptor CustomSchedulerSBR::findNode(string id) const
{
  graph::graph_t::vertex_iterator vit, vend;
  boost::tie(vit, vend) = boost::vertices(graph_);

  graph::graph_t::vertex_descriptor ret;
  for(; vit != vend; ++vit)
  {
    if(graph_[*vit]->cell()->name() == id)
      ret  = *vit;
  }
  
  return ret;
}

std::map<std::string, int> CustomSchedulerSBR::getDepthMap() const
{
  DepthType t = getDepthMap(getAllNodes());
  std::map<std::string, int> ret;
  
  for(DepthType::iterator it = t.begin(); it != t.end(); it++)
    ret[graph_[it->first]->cell()->name()] = it->second;
  
  return ret ;
}

CustomSchedulerSBR::DepthType CustomSchedulerSBR::getDepthMap(NodesVector vec) const
{
    DepthType ret;
    bool turn = false;

    do
    {
      turn = false;
      for(NodesVector::iterator it = vec.begin(); it != vec.end(); it++)
      {
        if(ret.count(*it) == 0)
          ret[*it] = -1;

        int deep = -1;

        for(graph::graph_t::adjacency_iterator it2 = adjacent_vertices(*it, graph_).first; it2 != adjacent_vertices(*it, graph_).second; it2++)
        {
          if(ret.count(*it2) == 0)
            ret[*it2] = -1;

          deep = max(deep, ret[*it2]);
        }

        if(deep+1 != ret[*it])
        {
          ret[*it] = deep+1;
          turn = true;
        }
      }
    }
    while(turn);

    return ret;
}

CustomSchedulerSBR::NodesVector CustomSchedulerSBR::getAllNodes() const
{
  NodesVector ret;
  
  for(graph::graph_t::vertex_iterator it = vertices(graph_).first; it != vertices(graph_).second; it++)
    ret.push_back(*it);
  
  return ret;
}

CustomSchedulerSBR::NodesVector CustomSchedulerSBR::getSubgraph(NodesVector vec)
{
  NodesVector ret;
  ret = vec;
  queue<graph::graph_t::vertex_descriptor> stack;
  
  for(NodesVector::iterator it = vec.begin(); it != vec.end(); it++)
    stack.push(*it);
  
  
  while(!stack.empty())
  {
    graph::graph_t::vertex_descriptor a = stack.front();
    stack.pop();
    for(graph::graph_t::adjacency_iterator it = adjacent_vertices(a, graph_).first; it != adjacent_vertices(a, graph_).second; it++)
    {
      if(count(ret.begin(), ret.end(), *it) == 0)
      {
        ret.push_back(*it);
        stack.push(*it);
      }
    }
  }
  
  return ret;
}

CustomSchedulerSBR::NodesVector CustomSchedulerSBR::getDepthNodes(DepthType m, int d) const
{
  NodesVector ret;
  
  for(DepthType::iterator it = m.begin(); it != m.end(); it++)
  {
    if(it->second == d)
      ret.push_back(it->first);
  }
  
  return ret;
}
