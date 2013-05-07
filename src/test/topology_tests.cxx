#include <geometry/entity.hxx>
#include <geometry/geometry.hxx>
#include <geometry/topology.hxx>
#include <file_io/ug_io_utility.hxx>

#include "gtest/gtest.h"

TEST(TopologyTest, Creation)
{
  Proteus::Geometry::node_list nl;
  Proteus::Geometry::face_list fl;

  nl.push(1.,2.,3.);
  nl.push(2.,3.,4.);
  nl.push(3.,4.,5.);
  nl.push(4.,5.,6.);
  fl.push(0,1,2);
  fl.push(1,2,3);

  Proteus::topology
  <
    Proteus::Geometry::node_list,
	Proteus::Geometry::face_list,
	Proteus::node_to_face_topo_ctr,
	std::vector<std::vector<std::size_t>>
  >
  node_to_face_topo(nl,fl);
  Proteus::node_to_face_topo_ctr ntftc;
}

TEST(TopologyTest, NodeToFaceConstructTrivial)
{
  Proteus::Geometry::node_list nl;
  Proteus::Geometry::face_list fl;

  nl.push(1.,2.,3.);
  nl.push(2.,3.,4.);
  nl.push(3.,4.,5.);
  nl.push(4.,5.,6.);
  fl.push(0,1,2);
  fl.push(1,2,3);

  Proteus::topology
  <
    Proteus::Geometry::node_list,
	Proteus::Geometry::face_list,
	Proteus::node_to_face_topo_ctr
  >
  node_to_face_topo(nl,fl);

  node_to_face_topo.construct();

  for(int i=0; i< nl.size(); ++i) {
	for(auto itr : node_to_face_topo[i]) {
	  EXPECT_EQ(true,
	  (i == fl[itr][0] ||
	   i == fl[itr][1] ||
	   i == fl[itr][2]));
	}
  }
}

TEST(TopologyTest, NodeToTriaContruct)
{
  Proteus::Geometry::node_list node_list;
  Proteus::Geometry::tria_list tria_list;
  Proteus::Geometry::quad_list quad_list;
  Proteus::Geometry::tet_list tet_list;
  Proteus::Geometry::pent5_list pent5_list;
  Proteus::Geometry::pent6_list pent6_list;
  Proteus::Geometry::hex_list hex_list;

  std::string file_name = "../test_data/horse.surf";
  EXPECT_NO_THROW(
  create_entity_lists_from_file_ug_io
  (file_name,
   node_list,
   tria_list,
   quad_list,
   tet_list,
   pent5_list,
   pent6_list,
   hex_list));

  Proteus::topology
  <
    Proteus::Geometry::node_list,
	Proteus::Geometry::tria_list,
	Proteus::node_to_tria_topo_ctr
  >
  node_to_tria_topo(node_list,tria_list);

  node_to_tria_topo.construct();

  for(int i=0; i< node_list.size(); ++i) {
	for(auto itr : node_to_tria_topo[i]) {
	  EXPECT_EQ(true,
	  (i == tria_list[itr][0] ||
	   i == tria_list[itr][1] ||
	   i == tria_list[itr][2]));
	}
  }
}

