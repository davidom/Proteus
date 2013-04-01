#include "node.hxx"
#include "gtest/gtest.h"

TEST(NodeTest, CreationWithIntializationList)
{
  double x, y, z;
  x = 1; y = 2; z = 3;
  Proteus::node n1({x,y,z});
  EXPECT_EQ(n1[0],x);
  EXPECT_EQ(n1[1],y);
  EXPECT_EQ(n1[2],z);
}

TEST(NodeTest, ThrowOutOfRange)
{
  double x, y, z;
  x = 1; y = 2; z = 3;
  Proteus::node n1({x,y,z});
  EXPECT_THROW(n1[3],std::exception);
}
