#include <geometry/entity.hxx>
#include <geometry/entity_list.hxx>
#include <geometry/entity_trait.hxx>
#include <geometry/geometry.hxx>
#include <geometry/entity_iterator.hxx>
#include "gtest/gtest.h"

#include <map>
#include <utility>

TEST(EntityIteratorTest, Creation)
{
  Proteus::entity_list<Proteus::Geometry::node3d> list;
  std::vector<size_t> range;

  Proteus::entity_iterator
  <
    Proteus::entity_list
	<
	  Proteus::Geometry::node3d
	>
	,
	std::vector<size_t>
  >
  ei(list,range);
}

TEST(EntityIteratorTest, DereferenceOperator)
{
  size_t index1=8, index2=3, index3=2, index4=6; 
  Proteus::entity_list<Proteus::Geometry::node3d> list;
  std::vector<size_t> range;

  // add some nodes to the list
  for(double n=0.; n<10; ++n) {
    list.push(Proteus::Geometry::node3d(n,n+1,n+2));
  }

  // add some values to the range
  range.push_back(index1);
  range.push_back(index2);
  range.push_back(index3);
  range.push_back(index4);

  Proteus::entity_iterator
  <
    Proteus::entity_list
	<
	  Proteus::Geometry::node3d
	>
	,
	std::vector<size_t>
  >
  ei(list,range);

  EXPECT_EQ((*ei)[0], index1);
  EXPECT_EQ((*ei)[1], index1+1);
  EXPECT_EQ((*ei)[2], index1+2);
}

TEST(EntityIteratorTest, ForwardIteration)
{
  size_t index1=8, index2=3, index3=2, index4=6; 
  Proteus::entity_list<Proteus::Geometry::node3d> list;
  std::vector<size_t> range;

  // add some nodes to the list
  for(double n=0.; n<10; ++n) {
    list.push(Proteus::Geometry::node3d(n,n+1,n+2));
  }

  // add some values to the range
  range.push_back(index1);
  range.push_back(index2);
  range.push_back(index3);
  range.push_back(index4);

  Proteus::entity_iterator
  <
    Proteus::entity_list
	<
	  Proteus::Geometry::node3d
	>
	,
	std::vector<size_t>
  >
  ei(list,range);

  ++ei;

  EXPECT_EQ((*ei)[0], index2);
  EXPECT_EQ((*ei)[1], index2+1);
  EXPECT_EQ((*ei)[2], index2+2);
}

TEST(EntityIteratorTest, Reset) 
{
  size_t index1=8, index2=3, index3=2, index4=6; 
  Proteus::entity_list<Proteus::Geometry::node3d> list;
  std::vector<size_t> range;

  // add some nodes to the list
  for(double n=0.; n<10; ++n) {
    list.push(Proteus::Geometry::node3d(n,n+1,n+2));
  }

  // add some values to the range
  range.push_back(index1);
  range.push_back(index2);
  range.push_back(index3);
  range.push_back(index4);

  Proteus::entity_iterator
  <
    Proteus::entity_list
	<
	  Proteus::Geometry::node3d
	>
	,
	std::vector<size_t>
  >
  ei(list,range);

  ++ei; ++ei; ++ei;

  EXPECT_NE((*ei)[0], index1);
  EXPECT_NE((*ei)[1], index1+1);
  EXPECT_NE((*ei)[2], index1+2);

  ei.reset();

  EXPECT_EQ((*ei)[0], index1);
  EXPECT_EQ((*ei)[1], index1+1);
  EXPECT_EQ((*ei)[2], index1+2);
}

TEST(EntityIteratorTest, BackwardIteration)
{
  size_t index1=8, index2=3, index3=2, index4=6; 
  Proteus::entity_list<Proteus::Geometry::node3d> list;
  std::vector<size_t> range;

  // add some nodes to the list
  for(double n=0.; n<10; ++n) {
    list.push(Proteus::Geometry::node3d(n,n+1,n+2));
  }

  // add some values to the range
  range.push_back(index1);
  range.push_back(index2);
  range.push_back(index3);
  range.push_back(index4);

  Proteus::entity_iterator
  <
    Proteus::entity_list
	<
	  Proteus::Geometry::node3d
	>
	,
	std::vector<size_t>
  >
  ei(list,range);

  ++ei;

  EXPECT_NE((*ei)[0], index1);
  EXPECT_NE((*ei)[1], index1+1);
  EXPECT_NE((*ei)[2], index1+2);

  --ei;

  EXPECT_EQ((*ei)[0], index1);
  EXPECT_EQ((*ei)[1], index1+1);
  EXPECT_EQ((*ei)[2], index1+2);
}

TEST(EntityIteratorTest, RangeBasedLoop)
{
  size_t index1=8, index2=3, index3=2, index4=6; 
  Proteus::entity_list<Proteus::Geometry::node3d> list;
  std::vector<size_t> range;

  // add some nodes to the list
  for(double n=0.; n<10; ++n) {
    list.push(Proteus::Geometry::node3d(n,n+1,n+2));
  }

  // add some values to the range
  range.push_back(index1);
  range.push_back(index2);
  range.push_back(index3);
  range.push_back(index4);

  Proteus::entity_iterator
  <
    Proteus::entity_list
	<
	  Proteus::Geometry::node3d
	>
	,
	std::vector<size_t>
  >
  ei(list,range);

  // this test requires that operator*, operator!=, .begin(), .end(), and operator++ work
  int i=0; 
  for ( auto eii : ei) { EXPECT_EQ(eii[0],range[i]); ++i;}

  auto eitr = ei.begin();
  auto eitr_end = ei.end();
  while(eitr != eitr_end) {
    EXPECT_EQ((*eitr)[0], eitr.index());

    ++eitr;
  }
}

TEST(EntityIteratorTest, IteratorWithAssociativeContainer)
{
  size_t index1=8, index2=3, index3=2, index4=6;
  double value1=10., value2=1., value3=7., value4=14.;

  std::map<size_t,double> et;
  Proteus::entity_list<Proteus::Geometry::node3d> list;
  Proteus::trait_adapter<std::map<size_t,double>> ta(et);
  Proteus::entity_iterator
  <
    Proteus::entity_list
	<
	  Proteus::Geometry::node3d
	>
	,
	Proteus::trait_adapter
	<
	  std::map<size_t,double>
	>
  >
  ei(list,ta);

  // add some nodes to the list
  for(double n=0.; n<10; ++n) {
    list.push(Proteus::Geometry::node3d(n,n+1,n+2));
  }

  // add some traits to the container
  et.insert(std::make_pair(index1, value1));
  et.insert(std::make_pair(index2, value2));
  et.insert(std::make_pair(index3, value3));
  et.insert(std::make_pair(index4, value4));

  for ( auto eii : ei) { ; }

  auto eitr = ei.begin();
  auto eitr_end = ei.end();
  while(eitr != eitr_end) {
    EXPECT_EQ((*eitr)[0], eitr.index());

    ++eitr;
  }
}

TEST(EntityIteratorTest, IteratorWithEntityTrait)
{
  size_t index1=8, index2=3, index3=2, index4=6;
  double value1=10., value2=1., value3=7., value4=14.;

  Proteus::entity_trait<double> et;
  Proteus::entity_list<Proteus::Geometry::node3d> list;
  Proteus::trait_adapter<Proteus::entity_trait<double>> ta(et);
  Proteus::entity_iterator
  <
    Proteus::entity_list
	<
	  Proteus::Geometry::node3d
	>
	,
	Proteus::trait_adapter
	<
	  Proteus::entity_trait<double>
	>
  >
  ei(list,ta);

  // add some nodes to the list
  for(double n=0.; n<10; ++n) {
    list.push(Proteus::Geometry::node3d(n,n+1,n+2));
  }

  // add some traits to the container
  et.set_trait(index1, value1);
  et.set_trait(index2, value2);
  et.set_trait(index3, value3);
  et.set_trait(index4, value4);

  // This is here because it won't compile if these operators aren't defined
  for ( auto eii : ei) { ; } 

  auto eitr = ei.begin();
  auto eitr_end = ei.end();
  while(eitr != eitr_end) {
    EXPECT_EQ((*eitr)[0], eitr.index());

    ++eitr;
  }
}
