#include <geometry/entity_trait.hxx>
#include "gtest/gtest.h"

TEST(EntityTraitTest, Creation)
{
  int size = 0;
  int value = 1;

  Proteus::entity_trait<int> et;
  EXPECT_EQ(et.size(),size);
}

TEST(EntityTraitTest, IsEqual)
{
  int index = 1;
  int value = 1;

  Proteus::entity_trait<int> et;

  EXPECT_EQ(et.is_equal(index,value), false);
}

TEST(EntityTraitTest, SetTrait)
{
  int index = 1, large_index = 100;
  int value = 1;

  Proteus::entity_trait<int> et;

  et.set_trait(index,value);
  EXPECT_EQ(et.is_equal(index,value), true);
  EXPECT_NO_THROW(et.is_equal(large_index,value));
  EXPECT_EQ(et.is_equal(index,value+1), false);
}

TEST(EntityTraitTest, UnsetTrait)
{
  int index = 1, large_index = 100;
  int value = 1;

  Proteus::entity_trait<int> et;

  et.set_trait(index,value);
  EXPECT_EQ(et.is_equal(index,value), true);
  et.unset_trait(index);
  EXPECT_EQ(et.is_equal(index,value), false);
  EXPECT_NO_THROW(et.unset_trait(large_index));
}


TEST(EntityTraitTest, UnsetEach)
{
  int index1 = 1, index2 = 2, index3 = 3;
  int value1 = 1, value2 = 2, value3 = 3;

  Proteus::entity_trait<int> et;

  et.set_trait(index1,value1);
  et.set_trait(index2,value2);
  et.set_trait(index3,value3);
  EXPECT_EQ(et.is_equal(index1,value1), true);
  EXPECT_EQ(et.is_equal(index2,value2), true);
  EXPECT_EQ(et.is_equal(index3,value3), true);
  et.unset_each();
  EXPECT_EQ(et.is_equal(index1,value1), false);
  EXPECT_EQ(et.is_equal(index2,value2), false);
  EXPECT_EQ(et.is_equal(index3,value3), false);
}

TEST(EntityTraitTest, SetAll)
{
  int index1 = 1, index2 = 2, index3 = 3;
  double value1 = 1, value2 = 2, value3 = 3, all_value = 4.0;

  Proteus::entity_trait<double> et;

  //set all values to all_value
  et.set_all(all_value);
  EXPECT_EQ(et.is_equal(index1,value1), false); //value1 != all_value
  EXPECT_EQ(et.is_equal(index3,all_value), true); //all_value for any index
}

TEST(EntityTraitTest, SetAllSetTraitInteraction)
{
  int index1 = 1, index2 = 2, index3 = 3;
  double value1 = 1, value2 = 2, value3 = 3, all_value = 4.0;

  Proteus::entity_trait<double> et;

  //set all values to all_value and then test that set_trait changes flag
  et.set_all(all_value);
  EXPECT_EQ(et.is_equal(index1,value1), false); //value1 != all_value
  EXPECT_EQ(et.is_equal(index3,all_value), true); //all_value for any index
  et.set_trait(index1, value1); //unset the all_value
  EXPECT_EQ(et.is_equal(index1,value1), true); //set correctly
  EXPECT_NE(et.is_equal(index3,all_value), true); //all_value is out of date
}

TEST(EntityTraitTest, SetAllUnsetTraitInteraction)
{
  int index1 = 1, index2 = 2, index3 = 3;
  double value1 = 1, value2 = 2, value3 = 3, all_value = 4.0;

  Proteus::entity_trait<double> et;

  //set all values to all_value and then test that unset_all changes flag
  et.set_all(all_value);
  EXPECT_EQ(et.is_equal(index1,value1), false); //value1 != all_value
  EXPECT_EQ(et.is_equal(index3,all_value), true); //all_value for any index

  //unset_trait with value that is not in trait has not effect on set_all flag
  et.unset_trait(index1);
  EXPECT_EQ(et.is_equal(index2,all_value), true); //all_value still good

  //set some trait
  et.set_trait(index1, value1); //put something in the trait
  et.set_all(all_value); //set_all with something in the trait
  et.unset_trait(index1); //unset a value that's in the trait 
  EXPECT_NE(et.is_equal(index2,all_value), true); //all_value out of date
}

TEST(EntityTraitTest, SetAllUnsetAllInteraction)
{
  int index1 = 1, index2 = 2, index3 = 3;
  double value1 = 1, value2 = 2, value3 = 3, all_value = 4.0;

  Proteus::entity_trait<double> et;

  //set all values to all_value and then test that unset_all changes flag
  et.set_all(all_value);
  EXPECT_EQ(et.is_equal(index1,value1), false); //value1 != all_value
  EXPECT_EQ(et.is_equal(index3,all_value), true); //all_value for any index
  et.unset_all();
  EXPECT_NE(et.is_equal(index2,all_value), true); //all_value is out of date

  //set some trait
  et.set_trait(index1,value1);
  et.set_all(all_value);
  EXPECT_EQ(et.is_equal(index1,value1), false); //value1 != all_value
  EXPECT_EQ(et.is_equal(index3,all_value), true); //all_value for any index
  et.unset_all();
  EXPECT_NE(et.is_equal(index2,all_value), true); //all_value is out of date
  EXPECT_EQ(et.is_equal(index1,value1), true); //trait existed before set_all
}

TEST(EntityTraitTest, SetAllUnsetEachInteraction)
{
  int index1 = 1, index2 = 2, index3 = 3;
  double value1 = 1, value2 = 2, value3 = 3, all_value = 4.0;

  Proteus::entity_trait<double> et;

  //set all values to all_value and then test that unset_each changes flag
  et.set_all(all_value);
  EXPECT_EQ(et.is_equal(index1,value1), false); //value1 != all_value
  EXPECT_EQ(et.is_equal(index3,all_value), true); //all_value for any index
  et.unset_each();
  EXPECT_NE(et.is_equal(index2,all_value), true); //all_value is out of date

  //set some trait
  et.set_trait(index1,value1);
  et.set_all(all_value);
  EXPECT_EQ(et.is_equal(index1,value1), false); //value1 != all_value
  EXPECT_EQ(et.is_equal(index3,all_value), true); //all_value for any index
  et.unset_each();
  EXPECT_NE(et.is_equal(index2,all_value), true); //all_value is out of date
  EXPECT_NE(et.is_equal(index1,value1), true); //traits cleared
}
