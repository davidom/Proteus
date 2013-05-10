#ifndef __PROTEUS_TOPOLOGY_CONSTRUCTOR_CLASS__
#define __PROTEUS_TOPOLOGY_CONSTRUCTOR_CLASS__

#include <geometry/topology.hxx>

namespace Proteus
{
  class node_to_face_topo_ctr
  {
    private:

	public:
	  template
	  <
	    typename node_list,
	    typename face_list
	  >
      void construct(Proteus::topology <> & t, const node_list &, const face_list &);
	  node_to_face_topo_ctr() = default;
  };

//#include <geometry/topology_constructor.cxx>

}
#endif
