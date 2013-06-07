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

  template
  <
    typename node_list,
    typename face_list
  >
  void
  Proteus::node_to_face_topo_ctr::
  construct(Proteus::topology <> & t, const node_list &nl, const face_list &fl)
  {
    //resize the map
    t.resize(nl.size());

    //fill in the map
    for(size_t i=0; i<fl.size(); ++i) {
      for(size_t j=0; j<fl[i].size(); ++j) {
	t.push(fl[i][j],i);
      }
    }
  }

}
#endif
