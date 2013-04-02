#include "face.hxx"
#include "gtest/gtest.h"

TEST(FaceTest, CreationWithIntializationList)
{
  size_t x, y, z;
  x = 1; y = 2; z = 3;
  Proteus::face f1({x,y,z});
  EXPECT_EQ(f1[0],x)<<"Initialized Value was not stored properly\n";
  EXPECT_EQ(f1[1],y)<<"Initialized Value was not stored properly\n";;
  EXPECT_EQ(f1[2],z)<<"Initialized Value was not stored properly\n";;
}

TEST(FaceTest, ThrowOutOfRange)
{
  size_t x, y, z;
  x = 1; y = 2; z = 3;
  Proteus::face f1({x,y,z});
  EXPECT_THROW(f1[3],std::exception)<<"Out of Range exception not thrown\n";
}

TEST(FaceTest, ZeroDimensionFace)
{
  EXPECT_NO_THROW(Proteus::face f1({}))<<"Cannot create empty std::initializer_list\n";
}

TEST(FaceTest, ZeroDimensionFaceOutOfRange)
{
  Proteus::face f1({});
  EXPECT_THROW(f1[0],std::exception)<<"Out of Range exception not thrown for empty std::initializer_list\n";
}
