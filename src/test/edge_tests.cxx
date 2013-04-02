#include "edge.hxx"
#include "gtest/gtest.h"

TEST(EdgeTest, CreationWithIntializationList)
{
  size_t x, y, z;
  x = 1; y = 2; z = 3;
  Proteus::edge e1({x,y,z});
  EXPECT_EQ(e1[0],x)<<"Initialized Value was not stored properly\n";
  EXPECT_EQ(e1[1],y)<<"Initialized Value was not stored properly\n";;
  EXPECT_EQ(e1[2],z)<<"Initialized Value was not stored properly\n";;
}

TEST(EdgeTest, ThrowOutOfRange)
{
  size_t x, y, z;
  x = 1; y = 2; z = 3;
  Proteus::edge e1({x,y,z});
  EXPECT_THROW(e1[3],std::exception)<<"Out of Range exception not thrown\n";
}

TEST(EdgeTest, ZeroDimensionEdge)
{
  EXPECT_NO_THROW(Proteus::edge e1({}))<<"Cannot create empty std::initializer_list\n";
}

TEST(EdgeTest, ZeroDimensionEdgeOutOfRange)
{
  Proteus::edge e1({});
  EXPECT_THROW(e1[0],std::exception)<<"Out of Range exception not thrown for empty std::initializer_list\n";
}
