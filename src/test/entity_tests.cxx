#include "entity.hxx"
#include "gtest/gtest.h"

#include <vector>
#include <array>
#include <map>

TEST(EntityTest, CreationVectorValue)
{
  int value = 1;
  Proteus::entity
    <Proteus::entity_impl<std::vector<int>>>
	e(value);

  EXPECT_EQ(e[0], value);
}

TEST(EntityTest, CreationVectorInitializerList)
{
  constexpr double x=1., y=2., z=3.;
  Proteus::entity
    <Proteus::entity_impl<std::vector<double>>>
	e(x,y,z);

  EXPECT_EQ(x, e[0]);
  EXPECT_EQ(y, e[1]);
  EXPECT_EQ(z, e[2]);
  EXPECT_NE(x, e[1]);
}

TEST(EntityTest, CreationArrayInitializerList)
{
  double x=1., y=2., z=3.;
  Proteus::entity
    <Proteus::entity_impl<std::array<double,3>>>
	e(x,y,z);

  EXPECT_EQ(x, e[0]);
  EXPECT_EQ(y, e[1]);
  EXPECT_EQ(z, e[2]);
  EXPECT_NE(x, e[1]);
}

TEST(EntityTest, CreationMapInitializerList)
{
  int key1 = 1, key2 = 1;
  double value = 2.;
  Proteus::entity
    <Proteus::entity_impl<std::map<int,double>>>
	e({key1,value},{key2,value});

}
