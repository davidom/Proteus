#include <geometry/entity.hxx>
#include <geometry/geometry.hxx>
#include "gtest/gtest.h"

#include <vector>
#include <array>
#include <map>
#include <typeinfo>

TEST(GeometryTest, Construction)
{
  const double x=1., y=2., z=3.;
  const int x1=1, y1=2, z1=3;

  Proteus::geometry<> g;
  auto node = g.create_node(x,y,z);
  auto edge = g.create_edge(x1,y1);
  auto face = g.create_face(x1,y1,z1);
  auto cell = g.create_cell(x1,y1,z1);

  Proteus::Geometry::node n = g.create_node(x,y,z);
  Proteus::Geometry::edge e = g.create_edge(x1,y1);
  Proteus::Geometry::face f = g.create_face(x1,y1,z1);
  Proteus::Geometry::cell c = g.create_cell(x1,y1,z1);
  
  EXPECT_EQ(typeid(node), typeid(n));
  EXPECT_EQ(typeid(edge), typeid(e));
  EXPECT_EQ(typeid(face), typeid(f));
  EXPECT_EQ(typeid(cell), typeid(c));
}
