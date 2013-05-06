#include <geometry/entity.hxx>
#include <geometry/geometry.hxx>
#include <geometry/topology.hxx>

#include "gtest/gtest.h"

TEST(TopologyTest, Creation)
{
  auto nl = new Proteus::Geometry::node_list;
  auto fl = new Proteus::Geometry::face_list;

  nl->push(1.,2.,3.);
  nl->push(2.,3.,4.);
  nl->push(3.,4.,5.);
  nl->push(4.,5.,6.);
  fl->push(0,1,2);
  fl->push(1,2,3);

  Proteus::topology
  <
    Proteus::Geometry::node_list,
	Proteus::Geometry::face_list,
	Proteus::node_to_face_topo_ctr,
	std::vector<std::vector<std::size_t>>
  >
  node_to_face_topo(*nl,*fl);
  Proteus::node_to_face_topo_ctr ntftc;
  ntftc.construct(node_to_face_topo);
}
