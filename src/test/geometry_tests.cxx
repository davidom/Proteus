#include <geometry/entity.hxx>
#include <geometry/geometry.hxx>
#include "gtest/gtest.h"

#include <vector>
#include <array>
#include <map>

TEST(GeometryTest, Construction)
{
  const double x=1., y=2., z=3.;
  const int x1=1, y1=2, z1=3;

  Proteus::geometry<> g;
  g.create_node(x,y,z);
  g.create_edge(x1,y1);
  g.create_face(x1,y1,z1);
  g.create_cell(x1,y1,z1);
  Proteus::Geometry::node3d n3(x,y,z);
  Proteus::Geometry::tria t(x1,y1,z1);
}
