#include <geometry/entity.hxx>
#include <geometry/entity_list.hxx>
#include <geometry/entity_trait.hxx>
#include <geometry/geometry.hxx>
#include <geometry/entity_iterator.hxx>
#include "gtest/gtest.h"

#include <map>
#include <utility>

TEST(ObverseEntityIteratorTest, IterationWithEntityTrait)
{
  size_t index1=0, index2=9, index3=2, index4=6;
  double value1=10., value2=1., value3=7., value4=14.;

  Proteus::Geometry::node3d_list list;
  Proteus::entity_trait<std::size_t,std::map<std::size_t,std::size_t>> et;
  Proteus::trait_adapter<decltype(et)> ta(et);
  Proteus::obverse_entity_iterator<decltype(list),decltype(ta)> ei(list, ta);

  // add some nodes to the list
  for(double n=0.; n<10; ++n) {
    list.push(Proteus::Geometry::node3d(n,n+1,n+2));
  }

  // add some traits to the container
  et.set_trait(index1, value1);
  et.set_trait(index2, value2);
  et.set_trait(index3, value3);
  et.set_trait(index4, value4);
  
  auto eitr = ei.begin();
  EXPECT_EQ(eitr.index(), 1);

  auto eitr_end = ei.end();
  EXPECT_EQ(eitr_end.index(), 8);
  while(eitr <= eitr_end) {
    EXPECT_NE(index1, eitr.index());
    EXPECT_NE(index2, eitr.index());
    EXPECT_NE(index3, eitr.index());
    EXPECT_NE(index4, eitr.index());

    ++eitr;
  }
}

TEST(ObverseEntityIteratorTest, IterationWithEmptyEntityTrait)
{
  size_t index1=0, index2=9, index3=2, index4=6;
  double value1=10., value2=1., value3=7., value4=14.;

  Proteus::Geometry::node3d_list list;
  Proteus::entity_trait<std::size_t,std::map<std::size_t,std::size_t>> et;
  Proteus::trait_adapter<decltype(et)> ta(et);
  Proteus::obverse_entity_iterator<decltype(list),decltype(ta)> ei(list, ta);

  // add some nodes to the list
  for(double n=0.; n<10; ++n) {
    list.push(Proteus::Geometry::node3d(n,n+1,n+2));
  }

  auto eitr = ei.begin();
  EXPECT_EQ(eitr.index(), 0);

  auto eitr_end = ei.end();
  EXPECT_EQ(eitr_end.index(), 9);

  while(eitr <= eitr_end) {
    EXPECT_EQ(eitr.index(), list[eitr.index()][0]);

    ++eitr;
  }
}

TEST(ObverseEntityIteratorTest, IterationWithFullEntityTrait)
{
  size_t index1=0, index2=9, index3=2, index4=6;
  double value1=10., value2=1., value3=7., value4=14.;

  Proteus::Geometry::node3d_list list;
  Proteus::entity_trait<std::size_t,std::map<std::size_t,std::size_t>> et;
  Proteus::trait_adapter<decltype(et)> ta(et);
  Proteus::obverse_entity_iterator<decltype(list),decltype(ta)> ei(list, ta);

  // add some nodes to the list
  for(double n=0.; n<10; ++n) {
    list.push(Proteus::Geometry::node3d(n,n+1,n+2));
    et.set_trait(n,n+2.71);
  }

  auto eitr = ei.begin();
  auto eitr_end = ei.end();

  EXPECT_GT(eitr.index(), eitr_end.index());
}

TEST(ObverseEntityIteratorTest, IterationWithOneEntityTraitFirst)
{
  size_t index1=0, index2=9, index3=2, index4=6;
  double value1=10., value2=1., value3=7., value4=14.;

  Proteus::Geometry::node3d_list list;
  Proteus::entity_trait<std::size_t,std::map<std::size_t,std::size_t>> et;
  Proteus::trait_adapter<decltype(et)> ta(et);
  Proteus::obverse_entity_iterator<decltype(list),decltype(ta)> ei(list, ta);

  // add some nodes to the list
  for(double n=0.; n<10; ++n) {
    list.push(Proteus::Geometry::node3d(n,n+1,n+2));
    if(n != 0)
      et.set_trait(n,n+2.71);
  }

  auto eitr = ei.begin();
  EXPECT_EQ(eitr.index(), 0);
  auto eitr_end = ei.end();
  EXPECT_EQ(eitr_end.index(), 0);

  while(eitr <= eitr_end) {
    EXPECT_EQ(eitr.index(), list[eitr.index()][0]);

    ++eitr;
  }
}

TEST(ObverseEntityIteratorTest, IterationWithOneEntityTraitLast)
{
  size_t index1=0, index2=9, index3=2, index4=6;
  double value1=10., value2=1., value3=7., value4=14.;

  Proteus::Geometry::node3d_list list;
  Proteus::entity_trait<std::size_t,std::map<std::size_t,std::size_t>> et;
  Proteus::trait_adapter<decltype(et)> ta(et);
  Proteus::obverse_entity_iterator<decltype(list),decltype(ta)> ei(list, ta);

  // add some nodes to the list
  for(double n=0.; n<10; ++n) {
    list.push(Proteus::Geometry::node3d(n,n+1,n+2));
    if(n != 9)
      et.set_trait(n,n+2.71);
  }

  auto eitr = ei.begin();
  EXPECT_EQ(eitr.index(), 9);
  auto eitr_end = ei.end();
  EXPECT_EQ(eitr_end.index(), 9);

  while(eitr <= eitr_end) {
    EXPECT_EQ(eitr.index(), list[eitr.index()][0]);

    ++eitr;
  }
}
