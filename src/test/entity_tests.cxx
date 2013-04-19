#include <geometry/entity.hxx>
#include <geometry/geometry.hxx>
#include "gtest/gtest.h"

#include <vector>
#include <array>
#include <map>

TEST(EntityTest, CreationVectorValue)
{
  int value = 1;
  Proteus::entity<std::vector<int>> e(value);

  EXPECT_EQ(e[0], value);
}

TEST(EntityTest, CreationVectorInitializerList)
{
  constexpr double x=1., y=2., z=3.;
  Proteus::entity<std::vector<double>> e(x,y,z);

  EXPECT_EQ(x, e[0]);
  EXPECT_EQ(y, e[1]);
  EXPECT_EQ(z, e[2]);
  EXPECT_NE(x, e[1]);
}

TEST(EntityTest, CreationArrayInitializerList)
{
  const double x=1., y=2., z=3.;
  Proteus::entity<std::array<double,3>> e(x,y,z);

  EXPECT_EQ(x, e[0]);
  EXPECT_EQ(y, e[1]);
  EXPECT_EQ(z, e[2]);
  EXPECT_NE(x, e[1]);
}

TEST(EntityTest, AssignmentOperator)
{
  const double x=1., y=2., z=3.;
  Proteus::entity<std::array<double,3>> a(x,y,z);

  Proteus::entity<std::array<double,3>> a1 = a;

  EXPECT_EQ(a1[0], a[0]);
  EXPECT_EQ(a1[1], a[1]);
  EXPECT_EQ(a1[2], a[2]);
  EXPECT_NE(a1[0], a[1]);
}

TEST(EntityTest, StoreInContainer)
{
  const double x=1., y=2., z=3.;
  Proteus::entity<std::array<double,3>> a(x,y,z);

  std::vector<Proteus::entity<std::array<double,3>>> v;
  v.insert(v.end(),100,Proteus::Geometry::node3d(x,y,z));
  v.insert(v.end(),100,a);
  v.emplace_back(a);

  EXPECT_EQ(v[0][0], a[0]);
  EXPECT_EQ(v[0][1], a[1]);
  EXPECT_EQ(v[0][2], a[2]);
  EXPECT_NE(v[0][0], a[1]);
}

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
