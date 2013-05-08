#ifndef __PROTEUS_TOPOLOGY_CONSTRUCTOR_CLASS__
#define __PROTEUS_TOPOLOGY_CONSTRUCTOR_CLASS__

#include <geometry/topology.hxx>

namespace Proteus
{
  class node_to_face_topo_ctr
  {
    private:

	public:
      void construct(
        Proteus::topology
        <
          Proteus::Geometry::node_list,
          Proteus::Geometry::face_list,
          Proteus::node_to_face_topo_ctr
        > & t
      );
	  node_to_face_topo_ctr() = default;
  };

  class node_to_tria_topo_ctr
  {
    private:

	public:
      void construct(
        Proteus::topology
        <
          Proteus::Geometry::node_list,
          Proteus::Geometry::tria_list,
          Proteus::node_to_tria_topo_ctr
        > & t
      );
	  node_to_tria_topo_ctr() = default;
  };

}
#endif
