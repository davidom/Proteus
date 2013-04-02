#include "cell.hxx"
#include "gtest/gtest.h"

TEST(CellTest, CreationWithIntializationList)
{
  size_t x, y, z;
  x = 1; y = 2; z = 3;
  Proteus::cell c1({x,y,z});
  EXPECT_EQ(c1[0],x)<<"Initialized Value was not stored properly\n";
  EXPECT_EQ(c1[1],y)<<"Initialized Value was not stored properly\n";;
  EXPECT_EQ(c1[2],z)<<"Initialized Value was not stored properly\n";;
}

TEST(CellTest, ThrowOutOfRange)
{
  size_t x, y, z;
  x = 1; y = 2; z = 3;
  Proteus::cell c1({x,y,z});
  EXPECT_THROW(c1[3],std::exception)<<"Out of Range exception not thrown\n";
}

TEST(CellTest, ZeroDimensionCell)
{
  EXPECT_NO_THROW(Proteus::cell c1({}))<<"Cannot create empty std::initializer_list\n";
}

TEST(CellTest, ZeroDimensionCellOutOfRange)
{
  Proteus::cell c1({});
  EXPECT_THROW(c1[0],std::exception)<<"Out of Range exception not thrown for empty std::initializer_list\n";
}
