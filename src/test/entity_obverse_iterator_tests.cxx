#include <geometry/entity.hxx>
#include <geometry/entity_list.hxx>
#include <geometry/entity_trait.hxx>
#include <geometry/geometry.hxx>
#include <geometry/entity_iterator.hxx>
#include "gtest/gtest.h"

#include <map>
#include <utility>

TEST(ObverseEntityIteratorTest, CreationWithAssociativeContainer)
{
  Proteus::Geometry::node3d_list list;
  std::map<size_t,double> et;
  Proteus::obverse_trait_adapter<decltype(et),decltype(list)> ta(et,list);
}

TEST(ObverseEntityIteratorTest, CreationWithEntityTrait)
{
  Proteus::Geometry::node3d_list list;
  Proteus::entity_trait<std::size_t> et;
  Proteus::obverse_trait_adapter<decltype(et),decltype(list)> ta(et,list);
}

TEST(ObverseEntityIteratorTest, IterationWithEntityTrait)
{
  size_t index1=8, index2=3, index3=2, index4=6;
  double value1=10., value2=1., value3=7., value4=14.;

  Proteus::Geometry::node3d_list list;
  Proteus::entity_trait<std::size_t,std::map<std::size_t,std::size_t>> et;
  Proteus::obverse_trait_adapter<decltype(et),decltype(list)> ta(et,list);
  Proteus::entity_iterator<decltype(list),decltype(ta)> ei(list, ta);

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
    EXPECT_NE(index1, eitr.index());
    EXPECT_NE(index2, eitr.index());
    EXPECT_NE(index3, eitr.index());
    EXPECT_NE(index4, eitr.index());

    ++eitr;
  }
}
