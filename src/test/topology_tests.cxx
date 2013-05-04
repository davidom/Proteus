#include <geometry/entity.hxx>
#include <geometry/geometry.hxx>
#include <geometry/topology.hxx>

#include "gtest/gtest.h"

TEST(TopologyTest, Creation)
{
  Proteus::Geometry::node_list nl;
  Proteus::Geometry::face_list fl;

  nl.push(1.,2.,3.);

  Proteus::topology
  <
    Proteus::Geometry::node_list,
	Proteus::Geometry::face_list,
	Proteus::node_to_face_topo_ctr
  >
  node_to_face_topo(nl,fl);

  Proteus::node_to_face_topo_ctr ntftc(node_to_face_topo);
  ntftc.construct();
}
