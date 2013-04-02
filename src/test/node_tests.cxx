#include "node.hxx"
#include "gtest/gtest.h"

TEST(NodeTest, CreationWithIntializationList)
{
  double x, y, z;
  x = 1; y = 2; z = 3;
  Proteus::node n1({x,y,z});
  EXPECT_EQ(n1[0],x)<<"Initialized Value was not stored properly\n";
  EXPECT_EQ(n1[1],y)<<"Initialized Value was not stored properly\n";;
  EXPECT_EQ(n1[2],z)<<"Initialized Value was not stored properly\n";;
}

TEST(NodeTest, ThrowOutOfRange)
{
  double x, y, z;
  x = 1; y = 2; z = 3;
  Proteus::node n1({x,y,z});
  EXPECT_THROW(n1[3],std::exception)<<"Out of Range exception not thrown\n";
}

TEST(NodeTest, ZeroDimensionNode)
{
  EXPECT_NO_THROW(Proteus::node n1({}))<<"Cannot create empty std::initializer_list\n";
}

TEST(NodeTest, ZeroDimensionNodeOutOfRange)
{
  Proteus::node n1({});
  EXPECT_THROW(n1[0],std::exception)<<"Out of Range exception not thrown for empty std::initializer_list\n";
}
