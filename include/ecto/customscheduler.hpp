#pragma once

#include <Python.h>
#include <ecto/log.hpp>
#include <ecto/forward.hpp>
#include <ecto/profile.hpp>

#include <boost/asio.hpp>
#ifndef BOOST_SIGNALS2_MAX_ARGS  // this is also defined in tendril.hpp (TODO: consolidate)
  #define BOOST_SIGNALS2_MAX_ARGS 3
#endif
#include <boost/signals2/signal.hpp>
#include <boost/thread/mutex.hpp>

#include <ecto/graph/types.hpp>
#include <ecto/scheduler.hpp>
#include <boost/graph/topological_sort.hpp>
#include <ecto/vertex.hpp>
#include <ecto/cell.hpp>
#include <ecto/util.hpp>
#include <list>
#include <utility>
#include <map>
#include <queue>
#include <algorithm>
#include <thread>

namespace ecto
{
  class ECTO_EXPORT CustomSchedulerSBR: public scheduler
  {
  public:
    typedef std::map<graph::graph_t::vertex_descriptor, int> DepthType;
    typedef std::vector<graph::graph_t::vertex_descriptor> NodesVector;
    
    explicit CustomSchedulerSBR(plasm_ptr p);
    ~CustomSchedulerSBR();
    
    using scheduler::execute;
    using scheduler::prepare_jobs;
    using scheduler::run_job;
    using scheduler::run;
    
    bool execute(std::vector<std::string>);
    bool execute_thread(std::vector<std::string>);
    
    bool run_thread();
    std::map<std::string, int> getDepthMap() const;
    
  protected:
    void compute_stack(NodesVector);
    
  private:
    std::vector<std::thread> m_threads;
    
    NodesVector getDepthNodes(DepthType, int = 0) const;   
    NodesVector getSubgraph(NodesVector);
    NodesVector getAllNodes() const;
    graph::graph_t::vertex_descriptor findNode(std::string) const;
    DepthType getDepthMap(NodesVector) const;
    
  };
}
