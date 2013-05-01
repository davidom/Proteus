#include <geometry/entity.hxx>
#include <geometry/geometry.hxx>
#include <geometry/entity_list.hxx>
#include "gtest/gtest.h"

TEST(EntityListTest, Construction)
{
  const double x=1., y=2., z=3.;

  Proteus::entity_list<Proteus::Geometry::node> el;
  el.push(x,y,z);
  EXPECT_EQ(1,el.size());

  Proteus::geometry<> g;
  el.push(g.create_node(x,y,z));

  EXPECT_EQ(2,el.size());
}

TEST(EntityListTest, ConstructionWithSize)
{
  const double x=1., y=2., z=3.;

  Proteus::entity_list<Proteus::Geometry::node3d> el(15);
  el.push(x,y,z);
  EXPECT_EQ(1,el.size());
}

TEST(EntityListTest, Access)
{
  const double x=1., y=2., z=3.;

  Proteus::entity_list<Proteus::Geometry::node> el1;
  el1.push(x,y,z);
  el1.push(z,x,y);
  EXPECT_EQ(el1[0][0],el1[1][1]);

  Proteus::entity_list<Proteus::Geometry::node> el2(10);
}
