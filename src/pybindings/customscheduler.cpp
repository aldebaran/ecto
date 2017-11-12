#include <ecto/scheduler.hpp>
#include <ecto/customscheduler.hpp>
#include <ecto/plasm.hpp>
#include <boost/python.hpp>

namespace bp = boost::python;


namespace ecto {
  namespace py {
    
    //BOOST_PYTHON_MODULE(ecto)
    //{
    
    
    template<class T>
    std::vector<T> toStdVector(bp::list l)
    {
      std::vector<T> ret;
      T p;
      for(int i = 0; i < len(l); i++)
      {
        p = bp::extract<T>(l[i]);
        ret.push_back(p);
      }
      return ret;
    }
    
    template <class K, class V>
    bp::dict toPythonDict(std::map<K, V> map) {
      typename std::map<K, V>::iterator iter;
      bp::dict dictionary;
      for (iter = map.begin(); iter != map.end(); ++iter) {
        dictionary[iter->first] = iter->second;
      }
      return dictionary;
    }
    
    template <typename T> bool execute0 (T& s) { return s.execute(); }
    template <typename T> bool execute1 (T& s, unsigned arg1)
    { return s.execute(arg1); }

    template <typename T> bool execute2 (T& s, bp::list id) { return s.execute(toStdVector<std::string>(id)); }
    // template <typename T> bool execute3 (T& s, bp::list id)
    // {
    //   std::vector<std::string> v = toStdVector<std::string>(id);

    //   return s.execute_thread(v);
      
    // }
        
    template <typename T> bool prepare_jobs0 (T& s)
    { return s.prepare_jobs(); }
    template <typename T> bool prepare_jobs1 (T& s, unsigned arg1)
    { return s.prepare_jobs(arg1); }

    template <typename T> bool run0 (T& s)
    { return s.run(); } 
    template <typename T> bool run1 (T& s, unsigned arg1)
    { return s.run(arg1); }

    // template <typename T> bool run2 (T& s)
    // { return s.run_thread(); }

    template <typename T> bp::dict depthMap (T& s)
    { return toPythonDict(s.getDepthMap()); }

    template <typename T> 
    void wrap_customscheduler(const char* name)
    {
      using bp::arg;
      bp::class_<T, boost::noncopyable>(name, bp::init<ecto::plasm::ptr>())
        .def("execute", &execute2<T>, arg("id"))
        .def("execute", &execute0<T>)
        .def("execute", &execute1<T>, arg("niter"))
        // .def("execute_thread", &execute3<T>, arg("id"))
        .def("prepare_jobs", &prepare_jobs0<T>)
        .def("prepare_jobs", &prepare_jobs1<T>, arg("niter"))
      
        //.def("interrupt", &T::interrupt)
        .def("stop", &T::stop)
        .def("running", (bool (scheduler::*)() const) &scheduler::running)
        //.def("wait", &T::wait)
        .def("run", &run0<T>)
        .def("run", &run1<T>, arg("timeout_usec"))
        // .def("run_thread", &run2<T>)
        .def("run_job", &T::run_job)
        .def("stats", &T::stats)
        .def("getDepthMap", &depthMap<T>)
      ;
    }

    void wrapCustomSchedulers()
    {
      //      bp::detail::init_module("ecto.CustomSchedulerSBR", initCustomSchedulerSBR);
      //bp::object CustomSchedulerSBR_module(bp::handle<>(bp::borrowed(PyImport_AddModule("ecto"))));
      //bp::scope().attr("CustomSchedulerSBR") = CustomSchedulers_module;
      //bp::scope CustomSchedulerSBR_scope = CustomSchedulerSBR_module;

      using bp::arg;

      wrap_customscheduler<ecto::CustomSchedulerSBR>("CustomSchedulerSBR");
    }
    //}
  }
}